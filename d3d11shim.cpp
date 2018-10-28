#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <algorithm>
#include <set>
#include <map>
#include <functional>
#include <iterator>

#include <windows.h>
#include <tlhelp32.h>
#include <delayimp.h>
#include <sddl.h>


EXTERN_C BOOL WINAPI
DllMainStartup(HANDLE hDllHandle, DWORD dwReason, LPVOID lpvReserved)
{
  if (dwReason == DLL_PROCESS_DETACH && lpvReserved) {

    return TRUE;
  }

  return TRUE;
}