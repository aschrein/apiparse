/*
 * Main program to start and inject a DLL into a process via a remote thread.
 *
 * For background see:
 * - http://en.wikipedia.org/wiki/DLL_injection#Approaches_on_Microsoft_Windows
 * - http://www.codeproject.com/KB/threads/completeinject.aspx
 * - http://www.codeproject.com/KB/threads/winspy.aspx
 * - http://www.codeproject.com/KB/DLL/DLL_Injection_tutorial.aspx
 * - http://www.codeproject.com/KB/threads/APIHooking.aspx
 *
 * Other slightly different techniques:
 * - http://www.fr33project.org/pages/projects/phook.htm
 * - http://www.hbgary.com/loading-a-dll-without-calling-loadlibrary
 * - http://securityxploded.com/ntcreatethreadex.php
 */

#include <string>
#include <stdio.h>
#include <assert.h>

#include <windows.h>
#include <psapi.h>
#include <dwmapi.h>
#include <tlhelp32.h>

//#include <getopt.h>

#ifndef ERROR_ELEVATION_REQUIRED
#define ERROR_ELEVATION_REQUIRED 740
#endif

//#include "os_version.hpp"
//#include "devcon.hpp"
#include "inject.h"


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif


static void
debugPrintf(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
}


static BOOL
attachDebugger(DWORD dwProcessId)
{
    long lRet;

    HKEY hKey;
    lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug", 0, KEY_READ, &hKey);
    if (lRet != ERROR_SUCCESS) {
        debugPrintf("inject: error: RegOpenKeyExA failed\n");
        return FALSE;
    }

    char szDebugger[1024];
    DWORD cbDebugger = sizeof szDebugger;
    lRet = RegQueryValueExA(hKey, "Debugger", NULL, NULL, (BYTE *)szDebugger, &cbDebugger);

    RegCloseKey(hKey);

    if (lRet != ERROR_SUCCESS) {
        if (lRet == ERROR_FILE_NOT_FOUND) {
            debugPrintf("inject: error: no automatic debugger configured\n");
        } else {
            debugPrintf("inject: error: RegQueryValueExA failed (0x%08lx)\n", lRet);
        }
        return FALSE;
    }

    SECURITY_ATTRIBUTES sa = { sizeof sa, 0, TRUE };
    HANDLE hEvent = CreateEvent(&sa, FALSE, FALSE, NULL);

    char szDebuggerCommand[1024];
    _snprintf(szDebuggerCommand, sizeof szDebuggerCommand, szDebugger,
              dwProcessId, (DWORD)(UINT_PTR)hEvent, NULL);

    debugPrintf("%s\n", szDebuggerCommand);

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof pi);
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof si);
    si.cb = sizeof &si;

    BOOL bRet = FALSE;
    if (!CreateProcessA(
           NULL,
           szDebuggerCommand,
           NULL, // process attributes
           NULL, // thread attributes
           TRUE, // inherit (event) handles
           0,
           NULL, // environment
           NULL, // current directory
           &si,
           &pi)) {
        debugPrintf("inject: error: failed to execute \"%s\" with 0x%08lx\n",
                    szDebuggerCommand,
                    GetLastError());
    } else {
        HANDLE handles[] = {
            hEvent,
            pi.hProcess
        };

        DWORD dwRet = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
        bRet = dwRet == WAIT_OBJECT_0;

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    CloseHandle(hEvent);

    return bRet;
}

static bool
isNumber(const char *arg) {
    while (*arg) {
        if (!isdigit(*arg++)) {
            return false;
        }
    }
    return true;
}


static BOOL
ejectDll(HANDLE hProcess, const char *szDllPath)
{
    /*
     * Enumerate all modules.
     */

    HMODULE *phModules = NULL;
    DWORD cb = sizeof *phModules *
#ifdef NDEBUG
        32
#else
        4
#endif
    ;
    DWORD cbNeeded = 0;
    while (true) {
        phModules = (HMODULE *)realloc(phModules, cb);
        if (!EnumProcessModules(hProcess, phModules, cb, &cbNeeded)) {
            logLastError("failed to enumerate modules in remote process");
            free(phModules);
            return FALSE;
        }

        if (cbNeeded < cb) {
            break;
        }

        cb *= 2;
    }

    DWORD cNumModules = cbNeeded / sizeof *phModules;

    /*
     * Search our DLL.
     */

    const char *szDllName = getBaseName(szDllPath);
    HMODULE hModule = NULL;
    for (unsigned i = 0; i < cNumModules; ++i) {
        char szModName[MAX_PATH];
        if (GetModuleFileNameExA(hProcess, phModules[i], szModName, ARRAY_SIZE(szModName))) {
            if (stricmp(getBaseName(szModName), szDllName) == 0) {
                hModule = phModules[i];
                break;
            }
        }
    }

    free(phModules);

    if (!hModule) {
        debugPrintf("inject: error: failed to find %s module in the remote process\n", szDllName);
        return FALSE;
    }

    PTHREAD_START_ROUTINE lpStartAddress =
        (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("KERNEL32"), "FreeLibrary");

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpStartAddress, hModule, 0, NULL);
    if (!hThread) {
        logLastError("failed to create remote thread");
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);

    DWORD bRet = 0;
    GetExitCodeThread(hThread, &bRet);
    if (!bRet) {
        debugPrintf("inject: error: failed to unload %s from the remote process\n", szDllPath);
        return FALSE;
    }

    return TRUE;
}


static void
help(void)
{
    fprintf(stderr,
            "usage:\n"
            "  inject -D <dllname.dll> <command> [args] ...\n"
            "  inject -D <dllname.dll> <process-id>\n"
            "  inject -D <dllname.dll> !<process-name>\n"
    );
}


static const char *short_options =
"hdD:mp:t:v";



int
main(int argc, char *argv[])
{
    BOOL bDebug = FALSE;
    BOOL bAttach = FALSE;
    DWORD dwProcessId = 0;
    DWORD dwThreadId = 0;
    char cVerbosity = 0;

    const char *szDllName = "injectee_iat.dll";
    const char *szDll = "d3d11shim.dll";

   

    HANDLE hSemaphore = NULL;
    if (!USE_SHARED_MEM) {
        SetEnvironmentVariableA("INJECT_DLL", szDll);
    } else {
        hSemaphore = CreateSemaphoreA(NULL, 1, 1, "inject_semaphore");
        if (hSemaphore == NULL) {
            debugPrintf("error: failed to create semaphore\n");
            return 1;
        }

        DWORD dwWait = WaitForSingleObject(hSemaphore, 0);
        if (dwWait == WAIT_TIMEOUT) {
            debugPrintf("info: waiting for another inject instance to finish\n");
            dwWait = WaitForSingleObject(hSemaphore, INFINITE);
        }
        if (dwWait != WAIT_OBJECT_0) {
            debugPrintf("error: failed to enter semaphore gate\n");
            return 1;
        }

        // Create a NULL DACL to enable the shared memory being accessed by any
        // process we attach to.
        SECURITY_DESCRIPTOR sd;
        SECURITY_DESCRIPTOR *lpSD = NULL;
        if (InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION) &&
            SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE))
        {
            lpSD = &sd;
        }

        SharedMem *pSharedMem = OpenSharedMemory(lpSD);
        if (!pSharedMem) {
            debugPrintf("error: failed to open shared memory\n");
            return 1;
        }

        pSharedMem->cVerbosity = cVerbosity;

        strncpy(pSharedMem->szDllName, szDll, _countof(pSharedMem->szDllName) - 1);
        pSharedMem->szDllName[_countof(pSharedMem->szDllName) - 1] = '\0';
    }

    BOOL bAttachDwm = FALSE;
    PROCESS_INFORMATION processInfo;
    HANDLE hProcess;
    if (bAttach) {
        BOOL bRet;
        HANDLE hToken   = NULL;
        bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
        if (!bRet) {
            debugPrintf("error: OpenProcessToken returned %u\n", (unsigned)bRet);
            return 1;
        }

        LUID Luid;
        bRet = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid);
        if (!bRet) {
            debugPrintf("error: LookupPrivilegeValue returned %u\n", (unsigned)bRet);
            return 1;
        }

        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = Luid;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        bRet = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof tp, NULL, NULL);
        if (!bRet) {
            debugPrintf("error: AdjustTokenPrivileges returned %u\n", (unsigned)bRet);
            return 1;
        }

        DWORD dwDesiredAccess =
            PROCESS_CREATE_THREAD |
            PROCESS_QUERY_INFORMATION |
            PROCESS_QUERY_LIMITED_INFORMATION |
            PROCESS_VM_OPERATION |
            PROCESS_VM_WRITE |
            PROCESS_VM_READ |
            PROCESS_TERMINATE;
        hProcess = OpenProcess(
            dwDesiredAccess,
            FALSE /* bInheritHandle */,
            dwProcessId);
        if (!hProcess) {
            logLastError("failed to open process");
            return 1;
        }

        char szProcess[MAX_PATH];
        DWORD dwRet = GetModuleFileNameExA(hProcess, 0, szProcess, sizeof szProcess);
        assert(dwRet);
        if (dwRet &&
            stricmp(getBaseName(szProcess), "dwm.exe") == 0) {
            bAttachDwm = TRUE;
        }
    } else {
      std::string commandLine = argv[1];// "example.exe";
        

        STARTUPINFOA startupInfo;
        memset(&startupInfo, 0, sizeof startupInfo);
        startupInfo.cb = sizeof startupInfo;

        // Create the process in suspended state
        if (!CreateProcessA(
               NULL,
               const_cast<char *>(commandLine.c_str()), // only modified by CreateProcessW
               0, // process attributes
               0, // thread attributes
               TRUE, // inherit handles
               CREATE_SUSPENDED,
               NULL, // environment
               NULL, // current directory
               &startupInfo,
               &processInfo)) {
            DWORD dwLastError = GetLastError();
            fprintf(stderr, "inject: error: failed to execute %s (%lu)\n",
                    commandLine.c_str(), dwLastError);
            if (dwLastError == ERROR_ELEVATION_REQUIRED) {
                fprintf(stderr, "error: target program requires elevated priviledges and must be started from an Administrator Command Prompt, or UAC must be disabled\n");
            }
            return 1;
        }

        hProcess = processInfo.hProcess;
    }

    if (isDifferentArch(hProcess)) {
        debugPrintf("error: binaries mismatch: you need to use the "
#ifdef _WIN64
                "32-bits"
#else
                "64-bits"
#endif
                " apitrace binaries to trace this application\n");
        TerminateProcess(hProcess, 1);
        return 1;
    }

    char szDllPath[MAX_PATH];
    GetModuleFileNameA(NULL, szDllPath, sizeof szDllPath);
    getDirName(szDllPath);
    strncat(szDllPath, szDllName, sizeof szDllPath - strlen(szDllPath) - 1);

    if (bDebug) {
        if (!attachDebugger(GetProcessId(hProcess))) {
            if (!bAttach) {
                TerminateProcess(hProcess, 1);
            }
            return 1;
        }
    }

#if 1
    if (!injectDll(hProcess, szDllPath)) {
        if (!bAttach) {
            TerminateProcess(hProcess, 1);
        }
        return 1;
    }
#endif

    DWORD exitCode;

    {
        // Start main process thread
        ResumeThread(processInfo.hThread);

        // Wait for it to finish
        WaitForSingleObject(hProcess, INFINITE);

        if (pSharedMem && !pSharedMem->bReplaced) {
            debugPrintf("warning: %s was never used: application probably does not use this API\n", szDll);
        }

        exitCode = ~0;
        GetExitCodeProcess(hProcess, &exitCode);

        CloseHandle(processInfo.hThread);
    }

    CloseHandle(hProcess);

    if (hSemaphore) {
        ReleaseSemaphore(hSemaphore, 1, NULL);
        CloseHandle(hSemaphore);
    }

    return (int)exitCode;
}
