 /*
  * Code for the DLL that will be injected in the target process.
  *
  * The injected DLL will manipulate the import tables to hook the
  * modules/functions of interest.
  *
  * See also:
  * - http://www.codeproject.com/KB/system/api_spying_hack.aspx
  * - http://www.codeproject.com/KB/threads/APIHooking.aspx
  * - http://msdn.microsoft.com/en-us/magazine/cc301808.aspx
  */

#define NOMINMAX

  
  //#define __STDC_WANT_LIB_EXT1__ 1
//#define _NO_CRT_STDIO_INLINE 1
#include <tchar.h>
#include <winsock2.h>
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
//#include <delayimp.h>

#include "inject.h"

#include <atomic>
#include <mutex>


#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <d3d11_3.h>
#include <d3d11_4.h>

extern HRESULT WrappedD3D11CreateDevice(
  IDXGIAdapter * pAdapter,
  D3D_DRIVER_TYPE DriverType,
  HMODULE Software,
  UINT Flags,
  const D3D_FEATURE_LEVEL * pFeatureLevels,
  UINT FeatureLevels,
  UINT SDKVersion,
  ID3D11Device ** ppDevice,
  D3D_FEATURE_LEVEL * pFeatureLevel,
  ID3D11DeviceContext ** ppImmediateContext
);
extern HRESULT WrappedD3D11CreateDeviceAndSwapChain(
  IDXGIAdapter * pAdapter,
  D3D_DRIVER_TYPE DriverType,
  HMODULE Software,
  UINT Flags,
  const D3D_FEATURE_LEVEL * pFeatureLevels,
  UINT FeatureLevels,
  UINT SDKVersion,
  const DXGI_SWAP_CHAIN_DESC * pSwapChainDesc,
  IDXGISwapChain ** ppSwapChain,
  ID3D11Device ** ppDevice,
  D3D_FEATURE_LEVEL * pFeatureLevel,
  ID3D11DeviceContext ** ppImmediateContext
);


static int VERBOSITY = 0;
#define NOOP 0


static CRITICAL_SECTION g_Mutex;



static HMODULE g_hThisModule = NULL;
static HMODULE g_hHookModule = NULL;


static void
debugPrintf(const char *format, ...)
{
  char buf[512];

  va_list ap;
  va_start(ap, format);
  _vsnprintf_s(buf, sizeof buf, format, ap);
  va_end(ap);

  OutputDebugStringA(buf);
}

#define ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))
#define VERBOSE_DEBUG_HOOK OPTION_OFF

using std::vector;
using std::map;
using std::string;
using std::wstring;

char toclower(char c)
{
  return (char)tolower(c);
}
wstring strlower(const wstring &str)
{
  wstring newstr(str);
  transform(newstr.begin(), newstr.end(), newstr.begin(), towlower);
  return newstr;
}
string strlower(const string &str)
{
  string newstr(str);
  transform(newstr.begin(), newstr.end(), newstr.begin(), toclower);
  return newstr;
}

struct LibraryHook;
typedef std::function<void(void *)> FunctionLoadCallback;

struct FunctionHook
{
  FunctionHook() : orig(NULL), hook(NULL) {}
  FunctionHook(const char *f, void **o, void *d) : function(f), orig(o), hook(d) {}
  bool operator<(const FunctionHook &h) const { return function < h.function; }
  std::string function;
  void **orig;
  void *hook;
};

class LibraryHooks
{
public:
  // generic, implemented in hooks.cpp to iterate over all registered libraries
  static void RegisterHooks();
  static void OptionsUpdated();

  // platform specific implementations

  // Removes hooks (where possible) and restores everything to an un-hooked state
  static void RemoveHooks();

  // refreshes hooks, useful on android where hooking can be unreliable
  static void Refresh();

  // Ignore this library - i.e. do not hook any calls it makes. Useful in the case where a library
  // might call in to hooked APIs but we want to treat it as a black box.
  static void IgnoreLibrary(const char *libraryName);

  // register a library for hooking, providing an optional callback to be called the first time the
  // library has been loaded and all functions in it hooked.
  static void RegisterLibraryHook(const char *libraryName, FunctionLoadCallback loadedCallback);

  // registers a function to be hooked, and an optional location of where to store the original
  // onward function pointer
  static void RegisterFunctionHook(const char *libraryName, const FunctionHook &hook);

  // detect if an identifier is present in the current process - used as a marker to indicate
  // replay-type programs.
  static bool Detect(const char *identifier);

private:
  static void BeginHookRegistration();
  static void EndHookRegistration();
};


// defines the interface that a library hooking class will implement.
struct LibraryHook
{
  LibraryHook();
  virtual void RegisterHooks() = 0;
  virtual void OptionsUpdated() {}
private:
  friend class LibraryHooks;
};

#if _WIN64

#define BIT_SPECIFIC_DLL(dll32, dll64) dll64

#else

#define BIT_SPECIFIC_DLL(dll32, dll64) dll32

#endif



template <typename FuncType>
class HookedFunction
{
public:
  HookedFunction() { orig_funcptr = NULL; }
  ~HookedFunction() {}
  FuncType operator()() { return (FuncType)orig_funcptr; }
  void SetFuncPtr(void *ptr) { orig_funcptr = ptr; }
  void Register(const char *module_name, const char *function, void *destination_function_ptr)
  {
    LibraryHooks::RegisterFunctionHook(
      module_name, FunctionHook(function, &orig_funcptr, destination_function_ptr));
  }

private:
  void *orig_funcptr;
};

class D3D11Hook : LibraryHook
{
public:
  D3D11Hook() {}
  void RegisterHooks()
  {

    // also require d3dcompiler_??.dll
    /*if (GetD3DCompiler() == NULL)
    {
    RDCERR("Failed to load d3dcompiler_??.dll - not inserting D3D11 hooks.");
    return;
    }*/

    LibraryHooks::RegisterLibraryHook("d3d11.dll", NULL);

    CreateDevice.Register("d3d11.dll", "D3D11CreateDevice", WrappedD3D11CreateDevice);
    CreateDeviceAndSwapChain.Register("d3d11.dll", "D3D11CreateDeviceAndSwapChain",
      WrappedD3D11CreateDeviceAndSwapChain);

  }

private:
  HookedFunction<PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN> CreateDeviceAndSwapChain;
  HookedFunction<PFN_D3D11_CREATE_DEVICE> CreateDevice;
  static D3D11Hook d3d11hooks;
};

D3D11Hook D3D11Hook::d3d11hooks;

class DXGIHook : LibraryHook
{
public:
  void RegisterHooks()
  {
    RDCLOG("Registering DXGI hooks");

    LibraryHooks::RegisterLibraryHook("dxgi.dll", NULL);

    CreateDXGIFactory.Register("dxgi.dll", "CreateDXGIFactory", CreateDXGIFactory_hook);
    CreateDXGIFactory1.Register("dxgi.dll", "CreateDXGIFactory1", CreateDXGIFactory1_hook);
    CreateDXGIFactory2.Register("dxgi.dll", "CreateDXGIFactory2", CreateDXGIFactory2_hook);
    GetDebugInterface.Register("dxgi.dll", "DXGIGetDebugInterface", DXGIGetDebugInterface_hook);
    GetDebugInterface1.Register("dxgi.dll", "DXGIGetDebugInterface1", DXGIGetDebugInterface1_hook);
  }

private:
  static DXGIHook dxgihooks;

  RenderDocAnalysis m_RenderDocAnalysis;

  HookedFunction<PFN_CREATE_DXGI_FACTORY> CreateDXGIFactory;
  HookedFunction<PFN_CREATE_DXGI_FACTORY> CreateDXGIFactory1;
  HookedFunction<PFN_CREATE_DXGI_FACTORY2> CreateDXGIFactory2;
  HookedFunction<PFN_GET_DEBUG_INTERFACE> GetDebugInterface;
  HookedFunction<PFN_GET_DEBUG_INTERFACE1> GetDebugInterface1;

  static HRESULT WINAPI CreateDXGIFactory_hook(__in REFIID riid, __out void **ppFactory)
  {
    if (ppFactory)
      *ppFactory = NULL;
    HRESULT ret = dxgihooks.CreateDXGIFactory()(riid, ppFactory);

    if (SUCCEEDED(ret))
      RefCountDXGIObject::HandleWrap(riid, ppFactory);

    return ret;
  }

  static HRESULT WINAPI CreateDXGIFactory1_hook(__in REFIID riid, __out void **ppFactory)
  {
    if (ppFactory)
      *ppFactory = NULL;
    HRESULT ret = dxgihooks.CreateDXGIFactory1()(riid, ppFactory);

    if (SUCCEEDED(ret))
      RefCountDXGIObject::HandleWrap(riid, ppFactory);

    return ret;
  }

  static HRESULT WINAPI CreateDXGIFactory2_hook(UINT Flags, REFIID riid, void **ppFactory)
  {
    if (ppFactory)
      *ppFactory = NULL;
    HRESULT ret = dxgihooks.CreateDXGIFactory2()(Flags, riid, ppFactory);

    if (SUCCEEDED(ret))
      RefCountDXGIObject::HandleWrap(riid, ppFactory);

    return ret;
  }

  static HRESULT WINAPI DXGIGetDebugInterface_hook(REFIID riid, void **ppDebug)
  {
    if (ppDebug)
      *ppDebug = NULL;

    if (riid == __uuidof(IDXGraphicsAnalysis))
    {
      dxgihooks.m_RenderDocAnalysis.AddRef();
      *ppDebug = &dxgihooks.m_RenderDocAnalysis;
      return S_OK;
    }

    if (dxgihooks.GetDebugInterface())
      return dxgihooks.GetDebugInterface()(riid, ppDebug);
    else
      return E_NOINTERFACE;
  }

  static HRESULT WINAPI DXGIGetDebugInterface1_hook(UINT Flags, REFIID riid, void **ppDebug)
  {
    if (ppDebug)
      *ppDebug = NULL;

    if (riid == __uuidof(IDXGraphicsAnalysis))
    {
      dxgihooks.m_RenderDocAnalysis.AddRef();
      *ppDebug = &dxgihooks.m_RenderDocAnalysis;
      return S_OK;
    }

    if (dxgihooks.GetDebugInterface1())
      return dxgihooks.GetDebugInterface1()(Flags, riid, ppDebug);
    else
      return E_NOINTERFACE;
  }
};

DXGIHook DXGIHook::dxgihooks;

static std::vector<LibraryHook *> &LibList()
{
  static std::vector<LibraryHook *> libs;
  return libs;
}

LibraryHook::LibraryHook()
{
  LibList().push_back(this);
}

void LibraryHooks::RegisterHooks()
{
  BeginHookRegistration();

  for (LibraryHook *lib : LibList())
    lib->RegisterHooks();

  EndHookRegistration();
}

void LibraryHooks::OptionsUpdated()
{
  for (LibraryHook *lib : LibList())
    lib->OptionsUpdated();
}

// map from address of IAT entry, to original contents
map<void **, void *> s_InstalledHooks;
std::mutex installedLock;

#define SCOPED_LOCK(m) std::lock_guard<std::mutex> gggggAAAAA(m)

bool ApplyHook(FunctionHook &hook, void **IATentry, bool &already)
{
  DWORD oldProtection = PAGE_EXECUTE;

  if (*IATentry == hook.hook)
  {
    already = true;
    return true;
  }

#if VERBOSE_DEBUG_HOOK
  debugPrintf("Patching IAT for %s: %p to %p", function.c_str(), IATentry, hookptr);
#endif

  {
    SCOPED_LOCK(installedLock);
    if (s_InstalledHooks.find(IATentry) == s_InstalledHooks.end())
      s_InstalledHooks[IATentry] = *IATentry;
  }

  BOOL success = TRUE;

  success = VirtualProtect(IATentry, sizeof(void *), PAGE_READWRITE, &oldProtection);
  if (!success)
  {
    debugPrintf("Failed to make IAT entry writeable 0x%p", IATentry);
    return false;
  }

  *IATentry = hook.hook;

  success = VirtualProtect(IATentry, sizeof(void *), oldProtection, &oldProtection);
  if (!success)
  {
    debugPrintf("Failed to restore IAT entry protection 0x%p", IATentry);
    return false;
  }

  return true;
}

struct DllHookset
{
  HMODULE module = NULL;
  bool hooksfetched = false;
  // if we have multiple copies of the dll loaded (unlikely), the other module handles will be
  // stored here
  vector<HMODULE> altmodules;
  vector<FunctionHook> FunctionHooks;
  DWORD OrdinalBase = 0;
  vector<string> OrdinalNames;
  std::vector<FunctionLoadCallback> Callbacks;
  std::mutex ordinallock;

  void FetchOrdinalNames()
  {
    SCOPED_LOCK(ordinallock);

    // return if we already fetched the ordinals
    if (!OrdinalNames.empty())
      return;

    byte *baseAddress = (byte *)module;

#if VERBOSE_DEBUG_HOOK
    debugPrintf("FetchOrdinalNames");
#endif

    PIMAGE_DOS_HEADER dosheader = (PIMAGE_DOS_HEADER)baseAddress;

    if (dosheader->e_magic != 0x5a4d)
      return;

    char *PE00 = (char *)(baseAddress + dosheader->e_lfanew);
    PIMAGE_FILE_HEADER fileHeader = (PIMAGE_FILE_HEADER)(PE00 + 4);
    PIMAGE_OPTIONAL_HEADER optHeader =
      (PIMAGE_OPTIONAL_HEADER)((BYTE *)fileHeader + sizeof(IMAGE_FILE_HEADER));

    DWORD eatOffset = optHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

    IMAGE_EXPORT_DIRECTORY *exportDesc = (IMAGE_EXPORT_DIRECTORY *)(baseAddress + eatOffset);

    WORD *ordinals = (WORD *)(baseAddress + exportDesc->AddressOfNameOrdinals);
    DWORD *names = (DWORD *)(baseAddress + exportDesc->AddressOfNames);

    DWORD count = std::min(exportDesc->NumberOfFunctions, exportDesc->NumberOfNames);

    WORD maxOrdinal = 0;
    for (DWORD i = 0; i < count; i++)
      maxOrdinal = std::max(maxOrdinal, ordinals[i]);

    OrdinalBase = exportDesc->Base;
    OrdinalNames.resize(maxOrdinal + 1);

    for (DWORD i = 0; i < count; i++)
    {
      OrdinalNames[ordinals[i]] = (char *)(baseAddress + names[i]);

#if VERBOSE_DEBUG_HOOK
      debugPrintf("ordinal found: '%s' %u", OrdinalNames[ordinals[i]].c_str(), (uint32_t)ordinals[i]);
#endif
    }
  }
};

struct CachedHookData
{
  CachedHookData()
  {
    ownmodule = NULL;
    missedOrdinals = false;
    memset(&lowername, 0, sizeof(lowername));
  }

  map<string, DllHookset> DllHooks;
  HMODULE ownmodule;
  std::mutex lock;
  char lowername[512];

  std::set<std::string> ignores;

  bool missedOrdinals;

  std::atomic<int32_t> posthooking = 0;

  void ApplyHooks(const char *modName, HMODULE module)
  {
    {
      size_t i = 0;
      while (modName[i])
      {
        lowername[i] = (char)tolower(modName[i]);
        i++;
      }
      lowername[i] = 0;
    }

    if (strstr(lowername, "injectee_iat.dll") == lowername)
      return;

#if VERBOSE_DEBUG_HOOK
    debugPrintf("=== ApplyHooks(%s, %p)", modName, module);
#endif

    // set module pointer if we are hooking exports from this module
    for (auto it = DllHooks.begin(); it != DllHooks.end(); ++it)
    {
      if (!_stricmp(it->first.c_str(), modName))
      {
        if (it->second.module == NULL)
        {
          it->second.module = module;

          it->second.hooksfetched = true;

          // fetch all function hooks here, since we want to fill out the original function pointer
          // even in case nothing imports from that function (which means it would not get filled
          // out through FunctionHook::ApplyHook)
          for (FunctionHook &hook : it->second.FunctionHooks)
          {
            if (hook.orig && *hook.orig == NULL)
              *hook.orig = GetProcAddress(module, hook.function.c_str());
          }

          it->second.FetchOrdinalNames();
        }
        else if (it->second.module != module)
        {
          // if it's already in altmodules, bail
          bool already = false;

          for (size_t i = 0; i < it->second.altmodules.size(); i++)
          {
            if (it->second.altmodules[i] == module)
            {
              already = true;
              break;
            }
          }

          if (already)
            break;

          // check if the previous module is still valid
          SetLastError(0);
          char filename[MAX_PATH] = {};
          GetModuleFileNameA(it->second.module, filename, MAX_PATH - 1);
          DWORD err = GetLastError();
          char *slash = strrchr(filename, L'\\');

          string basename = slash ? strlower(string(slash + 1)) : "";

          if (err == 0 && basename == it->first)
          {
            // previous module is still loaded, add this to the alt modules list
            it->second.altmodules.push_back(module);
          }
          else
          {
            // previous module is no longer loaded or there's a new file there now, add this as the
            // new location
            it->second.module = module;
          }
        }
      }
    }

    // for safety (and because we don't need to), ignore these modules
    if (!_stricmp(modName, "kernel32.dll") || !_stricmp(modName, "powrprof.dll") ||
      !_stricmp(modName, "CoreMessaging.dll") || !_stricmp(modName, "opengl32.dll") ||
      !_stricmp(modName, "gdi32.dll") || !_stricmp(modName, "gdi32full.dll") ||
      !_stricmp(modName, "nvoglv32.dll") || !_stricmp(modName, "nvoglv64.dll") ||
      !_stricmp(modName, "nvcuda.dll") || strstr(lowername, "cudart") == lowername ||
      strstr(lowername, "msvcr") == lowername || strstr(lowername, "msvcp") == lowername ||
      strstr(lowername, "nv-vk") == lowername || strstr(lowername, "amdvlk") == lowername ||
      strstr(lowername, "igvk") == lowername || strstr(lowername, "nvopencl") == lowername ||
      strstr(lowername, "nvapi") == lowername)
      return;

    if (ignores.find(lowername) != ignores.end())
      return;

    byte *baseAddress = (byte *)module;

    // the module could have been unloaded after our toolhelp snapshot, especially if we spent a
    // long time
    // dealing with a previous module (like adding our hooks).
    wchar_t modpath[1024] = { 0 };
    GetModuleFileNameW(module, modpath, 1023);
    if (modpath[0] == 0)
      return;
    // increment the module reference count, so it doesn't disappear while we're processing it
    // there's a very small race condition here between if GetModuleFileName returns, the module is
    // unloaded then we load it again. The only way around that is inserting very scary locks
    // between here
    // and FreeLibrary that I want to avoid. Worst case, we load a dll, hook it, then unload it
    // again.
    HMODULE refcountModHandle = LoadLibraryW(modpath);

    PIMAGE_DOS_HEADER dosheader = (PIMAGE_DOS_HEADER)baseAddress;

    if (dosheader->e_magic != 0x5a4d)
    {
      debugPrintf("Ignoring module %s, since magic is 0x%04x not 0x%04x", modName,
        (uint32_t)dosheader->e_magic, 0x5a4dU);
      FreeLibrary(refcountModHandle);
      return;
    }

    char *PE00 = (char *)(baseAddress + dosheader->e_lfanew);
    PIMAGE_FILE_HEADER fileHeader = (PIMAGE_FILE_HEADER)(PE00 + 4);
    PIMAGE_OPTIONAL_HEADER optHeader =
      (PIMAGE_OPTIONAL_HEADER)((BYTE *)fileHeader + sizeof(IMAGE_FILE_HEADER));

    DWORD iatOffset = optHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

    IMAGE_IMPORT_DESCRIPTOR *importDesc = (IMAGE_IMPORT_DESCRIPTOR *)(baseAddress + iatOffset);

#if VERBOSE_DEBUG_HOOK
    debugPrintf("=== import descriptors:");
#endif

    while (iatOffset && importDesc->FirstThunk)
    {
      const char *dllName = (const char *)(baseAddress + importDesc->Name);

#if VERBOSE_DEBUG_HOOK
      debugPrintf("found IAT for %s", dllName);
#endif

      DllHookset *hookset = NULL;

      for (auto it = DllHooks.begin(); it != DllHooks.end(); ++it)
        if (!_stricmp(it->first.c_str(), dllName))
          hookset = &it->second;

      if (hookset && importDesc->OriginalFirstThunk > 0 && importDesc->FirstThunk > 0)
      {
        IMAGE_THUNK_DATA *origFirst =
          (IMAGE_THUNK_DATA *)(baseAddress + importDesc->OriginalFirstThunk);
        IMAGE_THUNK_DATA *first = (IMAGE_THUNK_DATA *)(baseAddress + importDesc->FirstThunk);

#if VERBOSE_DEBUG_HOOK
        debugPrintf("Hooking imports for %s", dllName);
#endif

        while (origFirst->u1.AddressOfData)
        {
          void **IATentry = (void **)&first->u1.AddressOfData;

          struct hook_find
          {
            bool operator()(const FunctionHook &a, const char *b)
            {
              return _stricmp(a.function.c_str(), b) < 0;
            }
          };

#if _WIN64
          if (IMAGE_SNAP_BY_ORDINAL64(origFirst->u1.AddressOfData))
#else
          if (IMAGE_SNAP_BY_ORDINAL32(origFirst->u1.AddressOfData))
#endif
          {
            // low bits of origFirst->u1.AddressOfData contain an ordinal
            WORD ordinal = IMAGE_ORDINAL64(origFirst->u1.AddressOfData);

#if VERBOSE_DEBUG_HOOK
            debugPrintf("Found ordinal import %u", (uint32_t)ordinal);
#endif

            if (!hookset->OrdinalNames.empty())
            {
              if (ordinal >= hookset->OrdinalBase)
              {
                // rebase into OrdinalNames index
                DWORD nameIndex = ordinal - hookset->OrdinalBase;

                // it's perfectly valid to have more functions than names, we only
                // list those with names - so ignore any others
                if (nameIndex < hookset->OrdinalNames.size())
                {
                  const char *importName = (const char *)hookset->OrdinalNames[nameIndex].c_str();

#if VERBOSE_DEBUG_HOOK
                  debugPrintf("Located ordinal %u as %s", (uint32_t)ordinal, importName);
#endif

                  auto found =
                    std::lower_bound(hookset->FunctionHooks.begin(), hookset->FunctionHooks.end(),
                      importName, hook_find());

                  if (found != hookset->FunctionHooks.end() &&
                    !_stricmp(found->function.c_str(), importName) && ownmodule != module)
                  {
                    bool already = false;
                    bool applied;
                    {
                      SCOPED_LOCK(lock);
                      applied = ApplyHook(*found, IATentry, already);
                    }

                    // if we failed, or if it's already set and we're not doing a missedOrdinals
                    // second pass, then just bail out immediately as we've already hooked this
                    // module and there's no point wasting time re-hooking nothing
                    if (!applied || (already && !missedOrdinals))
                    {
#if VERBOSE_DEBUG_HOOK
                      debugPrintf("Stopping hooking module, %d %d %d", (int)applied, (int)already,
                        (int)missedOrdinals);
#endif
                      FreeLibrary(refcountModHandle);
                      return;
                    }
                  }
                }
              }
              else
              {
                debugPrintf("Import ordinal is below ordinal base in %s importing module %s", modName,
                  dllName);
              }
            }
            else
            {
#if VERBOSE_DEBUG_HOOK
              debugPrintf("missed ordinals, will try again");
#endif
              // the very first time we try to apply hooks, we might apply them to a module
              // before we've looked up the ordinal names for the one it's linking against.
              // Subsequent times we're only loading one new module - and since it can't
              // link to itself we will have all ordinal names loaded.
              //
              // Setting this flag causes us to do a second pass right at the start
              missedOrdinals = true;
            }

            // continue
            origFirst++;
            first++;
            continue;
          }

          IMAGE_IMPORT_BY_NAME *import =
            (IMAGE_IMPORT_BY_NAME *)(baseAddress + origFirst->u1.AddressOfData);

          const char *importName = (const char *)import->Name;

#if VERBOSE_DEBUG_HOOK
          debugPrintf("Found normal import %s", importName);
#endif

          auto found = std::lower_bound(hookset->FunctionHooks.begin(),
            hookset->FunctionHooks.end(), importName, hook_find());

          if (found != hookset->FunctionHooks.end() &&
            !_stricmp(found->function.c_str(), importName) && ownmodule != module)
          {
            bool already = false;
            bool applied;
            {
              SCOPED_LOCK(lock);
              applied = ApplyHook(*found, IATentry, already);
            }

            // if we failed, or if it's already set and we're not doing a missedOrdinals
            // second pass, then just bail out immediately as we've already hooked this
            // module and there's no point wasting time re-hooking nothing
            if (!applied || (already && !missedOrdinals))
            {
#if VERBOSE_DEBUG_HOOK
              debugPrintf("Stopping hooking module, %d %d %d", (int)applied, (int)already,
                (int)missedOrdinals);
#endif
              FreeLibrary(refcountModHandle);
              return;
            }
          }

          origFirst++;
          first++;
        }
      }
      else
      {
        if (hookset)
        {
#if VERBOSE_DEBUG_HOOK
          debugPrintf("!! Invalid IAT found for %s! %u %u", dllName, importDesc->OriginalFirstThunk,
            importDesc->FirstThunk);
#endif
        }
      }

      importDesc++;
    }

    FreeLibrary(refcountModHandle);
  }
};

static CachedHookData *s_HookData = NULL;

#ifdef UNICODE
#undef MODULEENTRY32
#undef Module32First
#undef Module32Next
#endif

static void ForAllModules(std::function<void(const MODULEENTRY32 &me32)> callback)
{
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;

  // up to 10 retries
  for (int i = 0; i < 10; i++)
  {
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
      DWORD err = GetLastError();

      debugPrintf("CreateToolhelp32Snapshot() -> 0x%08x", err);

      // retry if error is ERROR_BAD_LENGTH
      if (err == ERROR_BAD_LENGTH)
        continue;
    }

    // didn't retry, or succeeded
    break;
  }

  if (hModuleSnap == INVALID_HANDLE_VALUE)
  {
    debugPrintf("Couldn't create toolhelp dump of modules in process");
    return;
  }

  MODULEENTRY32 me32;
  memset(&me32, 0, sizeof(me32));
  me32.dwSize = sizeof(MODULEENTRY32);

  BOOL success = Module32First(hModuleSnap, &me32);

  if (success == FALSE)
  {
    DWORD err = GetLastError();

    debugPrintf("Couldn't get first module in process: 0x%08x", err);
    CloseHandle(hModuleSnap);
    return;
  }

  uintptr_t ret = 0;

  do
  {
    callback(me32);
  } while (ret == 0 && Module32Next(hModuleSnap, &me32));

  CloseHandle(hModuleSnap);
}

static void HookAllModules()
{
  ForAllModules(
    [](const MODULEENTRY32 &me32) { s_HookData->ApplyHooks(me32.szModule, me32.hModule); });

  // check if we're already in this section of code, and if so don't go in again.
  int32_t prev = (int32_t)InterlockedCompareExchange((volatile LONG *)&s_HookData->posthooking, 0, 1);

  if (prev != 0)
    return;

  // for all loaded modules, call callbacks now
  for (auto it = s_HookData->DllHooks.begin(); it != s_HookData->DllHooks.end(); ++it)
  {
    if (it->second.module == NULL)
      continue;

    if (!it->second.hooksfetched)
    {
      it->second.hooksfetched = true;

      // fetch all function hooks here, if we didn't above (perhaps because this library was
      // late-loaded)
      for (FunctionHook &hook : it->second.FunctionHooks)
      {
        if (hook.orig && *hook.orig == NULL)
          *hook.orig = GetProcAddress(it->second.module, hook.function.c_str());
      }
    }

    std::vector<FunctionLoadCallback> callbacks;
    // don't call callbacks next time
    callbacks.swap(it->second.Callbacks);

    for (FunctionLoadCallback cb : callbacks)
      if (cb)
        cb(it->second.module);
  }

  (int32_t)InterlockedCompareExchange((volatile LONG *)&s_HookData->posthooking, 1, 0);
}

static bool IsAPISet(const wchar_t *filename)
{
  if (wcschr(filename, L'/') != 0 || wcschr(filename, L'\\') != 0)
    return false;

  wchar_t match[] = L"api-ms-win";

  if (wcslen(filename) < ARRAY_COUNT(match) - 1)
    return false;

  for (size_t i = 0; i < ARRAY_COUNT(match) - 1; i++)
    if (towlower(filename[i]) != match[i])
      return false;

  return true;
}

static bool IsAPISet(const char *filename)
{
  std::wstring wfn;
  // assume ASCII not UTF, just upcast plainly to wchar_t
  while (*filename)
    wfn.push_back(wchar_t(*filename++));
  return IsAPISet(wfn.c_str());
}

HMODULE WINAPI Hooked_LoadLibraryExA(LPCSTR lpLibFileName, HANDLE fileHandle, DWORD flags)
{
  bool dohook = true;
  if (flags == 0 && GetModuleHandleA(lpLibFileName))
    dohook = false;

  SetLastError(S_OK);

  // we can use the function naked, as when setting up the hook for LoadLibraryExA, our own module
  // was excluded from IAT patching
  HMODULE mod = LoadLibraryExA(lpLibFileName, fileHandle, flags);

#if VERBOSE_DEBUG_HOOK
  debugPrintf("LoadLibraryA(%s)", lpLibFileName);
#endif

  DWORD err = GetLastError();

  if (dohook && mod && !IsAPISet(lpLibFileName))
    HookAllModules();

  SetLastError(err);

  return mod;
}

HMODULE WINAPI Hooked_LoadLibraryExW(LPCWSTR lpLibFileName, HANDLE fileHandle, DWORD flags)
{
  bool dohook = true;
  if (flags == 0 && GetModuleHandleW(lpLibFileName))
    dohook = false;

  SetLastError(S_OK);

#if VERBOSE_DEBUG_HOOK
  debugPrintf("LoadLibraryW(%ls)", lpLibFileName);
#endif

  // we can use the function naked, as when setting up the hook for LoadLibraryExA, our own module
  // was excluded from IAT patching
  HMODULE mod = LoadLibraryExW(lpLibFileName, fileHandle, flags);

  DWORD err = GetLastError();

  if (dohook && mod && !IsAPISet(lpLibFileName))
    HookAllModules();

  SetLastError(err);

  return mod;
}

HMODULE WINAPI Hooked_LoadLibraryA(LPCSTR lpLibFileName)
{
  return Hooked_LoadLibraryExA(lpLibFileName, NULL, 0);
}

HMODULE WINAPI Hooked_LoadLibraryW(LPCWSTR lpLibFileName)
{
  return Hooked_LoadLibraryExW(lpLibFileName, NULL, 0);
}

static bool OrdinalAsString(void *func)
{
  return uint64_t(func) <= 0xffff;
}

FARPROC WINAPI Hooked_GetProcAddress(HMODULE mod, LPCSTR func)
{
  if (mod == NULL || func == NULL)
    return (FARPROC)NULL;

  if (mod == s_HookData->ownmodule)
    return GetProcAddress(mod, func);

#if VERBOSE_DEBUG_HOOK
  if (OrdinalAsString((void *)func))
    debugPrintf("Hooked_GetProcAddress(%p, %p)", mod, func);
  else
    debugPrintf("Hooked_GetProcAddress(%p, %s)", mod, func);
#endif

  for (auto it = s_HookData->DllHooks.begin(); it != s_HookData->DllHooks.end(); ++it)
  {
    if (it->second.module == NULL)
    {
      it->second.module = GetModuleHandleA(it->first.c_str());
      if (it->second.module)
      {
        // fetch all function hooks here, since we want to fill out the original function pointer
        // even in case nothing imports from that function (which means it would not get filled
        // out through FunctionHook::ApplyHook)
        for (FunctionHook &hook : it->second.FunctionHooks)
        {
          if (hook.orig && *hook.orig == NULL)
            *hook.orig = GetProcAddress(it->second.module, hook.function.c_str());
        }

        it->second.FetchOrdinalNames();
      }
    }

    bool match = (mod == it->second.module);

    if (!match && !it->second.altmodules.empty())
    {
      for (size_t i = 0; !match && i < it->second.altmodules.size(); i++)
        match = (mod == it->second.altmodules[i]);
    }

    if (match)
    {
#if VERBOSE_DEBUG_HOOK
      debugPrintf("Located module %s", it->first.c_str());
#endif

      if (OrdinalAsString((void *)func))
      {
#if VERBOSE_DEBUG_HOOK
        debugPrintf("Ordinal hook");
#endif

        uint32_t ordinal = (uint16_t)(uintptr_t(func) & 0xffff);

        if (ordinal < it->second.OrdinalBase)
        {
          debugPrintf("Unexpected ordinal - lower than ordinalbase %u for %s",
            (uint32_t)it->second.OrdinalBase, it->first.c_str());

          SetLastError(S_OK);
          return GetProcAddress(mod, func);
        }

        ordinal -= it->second.OrdinalBase;

        if (ordinal >= it->second.OrdinalNames.size())
        {
          debugPrintf("Unexpected ordinal - higher than fetched ordinal names (%u) for %s",
            (uint32_t)it->second.OrdinalNames.size(), it->first.c_str());

          SetLastError(S_OK);
          return GetProcAddress(mod, func);
        }

        func = it->second.OrdinalNames[ordinal].c_str();

#if VERBOSE_DEBUG_HOOK
        debugPrintf("found ordinal %s", func);
#endif
      }

      FunctionHook search(func, NULL, NULL);

      auto found =
        std::lower_bound(it->second.FunctionHooks.begin(), it->second.FunctionHooks.end(), search);
      if (found != it->second.FunctionHooks.end() && !(search < *found))
      {
        FARPROC realfunc = GetProcAddress(mod, func);

#if VERBOSE_DEBUG_HOOK
        debugPrintf("Found hooked function, returning hook pointer %p", found->hookptr);
#endif

        SetLastError(S_OK);

        if (realfunc == NULL)
          return NULL;

        return (FARPROC)found->hook;
      }
    }
  }

#if VERBOSE_DEBUG_HOOK
  debugPrintf("No matching hook found, returning original");
#endif

  SetLastError(S_OK);

  return GetProcAddress(mod, func);
}

void LibraryHooks::RegisterFunctionHook(const char *libraryName, const FunctionHook &hook)
{
  if (!_stricmp(libraryName, "kernel32.dll"))
  {
    if (hook.function == "LoadLibraryA" || hook.function == "LoadLibraryW" ||
      hook.function == "LoadLibraryExA" || hook.function == "LoadLibraryExW" ||
      hook.function == "GetProcAddress")
    {
      debugPrintf("Cannot hook LoadLibrary* or GetProcAddress, as these are hooked internally");
      return;
    }
  }
  s_HookData->DllHooks[strlower(string(libraryName))].FunctionHooks.push_back(hook);
}

void LibraryHooks::RegisterLibraryHook(const char *libraryName, FunctionLoadCallback loadedCallback)
{
  s_HookData->DllHooks[strlower(string(libraryName))].Callbacks.push_back(loadedCallback);
}

void LibraryHooks::IgnoreLibrary(const char *libraryName)
{
  std::string lowername = libraryName;

  for (size_t i = 0; i < lowername.size(); i++)
    lowername[i] = (char)tolower(lowername[i]);

  s_HookData->ignores.insert(lowername);
}

void LibraryHooks::BeginHookRegistration()
{
  s_HookData = new CachedHookData;
  assert(s_HookData->DllHooks.empty());
  s_HookData->DllHooks["kernel32.dll"].FunctionHooks.push_back(
    FunctionHook("LoadLibraryA", NULL, &Hooked_LoadLibraryA));
  s_HookData->DllHooks["kernel32.dll"].FunctionHooks.push_back(
    FunctionHook("LoadLibraryW", NULL, &Hooked_LoadLibraryW));
  s_HookData->DllHooks["kernel32.dll"].FunctionHooks.push_back(
    FunctionHook("LoadLibraryExA", NULL, &Hooked_LoadLibraryExA));
  s_HookData->DllHooks["kernel32.dll"].FunctionHooks.push_back(
    FunctionHook("LoadLibraryExW", NULL, &Hooked_LoadLibraryExW));
  s_HookData->DllHooks["kernel32.dll"].FunctionHooks.push_back(
    FunctionHook("GetProcAddress", NULL, &Hooked_GetProcAddress));

  for (const char *apiset :
    { "api-ms-win-core-libraryloader-l1-1-0.dll", "api-ms-win-core-libraryloader-l1-1-1.dll",
    "api-ms-win-core-libraryloader-l1-1-2.dll", "api-ms-win-core-libraryloader-l1-2-0.dll",
    "api-ms-win-core-libraryloader-l1-2-1.dll" })
  {
    s_HookData->DllHooks[apiset].FunctionHooks.push_back(
      FunctionHook("LoadLibraryA", NULL, &Hooked_LoadLibraryA));
    s_HookData->DllHooks[apiset].FunctionHooks.push_back(
      FunctionHook("LoadLibraryW", NULL, &Hooked_LoadLibraryW));
    s_HookData->DllHooks[apiset].FunctionHooks.push_back(
      FunctionHook("LoadLibraryExA", NULL, &Hooked_LoadLibraryExA));
    s_HookData->DllHooks[apiset].FunctionHooks.push_back(
      FunctionHook("LoadLibraryExW", NULL, &Hooked_LoadLibraryExW));
    s_HookData->DllHooks[apiset].FunctionHooks.push_back(
      FunctionHook("GetProcAddress", NULL, &Hooked_GetProcAddress));
  }

  GetModuleHandleEx(
    GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
    (LPCTSTR)&s_HookData, &s_HookData->ownmodule);
}

// hook all functions for currently loaded modules.
// some of these hooks (as above) will hook LoadLibrary/GetProcAddress, to protect
void LibraryHooks::EndHookRegistration()
{
  for (auto it = s_HookData->DllHooks.begin(); it != s_HookData->DllHooks.end(); ++it)
    std::sort(it->second.FunctionHooks.begin(), it->second.FunctionHooks.end());

#if VERBOSE_DEBUG_HOOK
  debugPrintf("Applying hooks");
#endif

  HookAllModules();

  if (s_HookData->missedOrdinals)
  {
#if VERBOSE_DEBUG_HOOK
    debugPrintf("Missed ordinals - applying hooks again");
#endif

    // we need to do a second pass now that we know ordinal names to finally hook
    // some imports by ordinal only.
    HookAllModules();

    s_HookData->missedOrdinals = false;
  }
}

void LibraryHooks::Refresh()
{
  // don't need to refresh on windows
}

void LibraryHooks::RemoveHooks()
{
  for (auto it = s_InstalledHooks.begin(); it != s_InstalledHooks.end(); ++it)
  {
    DWORD oldProtection = PAGE_EXECUTE;
    BOOL success = TRUE;

    void **IATentry = it->first;

    success = VirtualProtect(IATentry, sizeof(void *), PAGE_READWRITE, &oldProtection);
    if (!success)
    {
      debugPrintf("Failed to make IAT entry writeable 0x%p", IATentry);
      continue;
    }

    *IATentry = it->second;

    success = VirtualProtect(IATentry, sizeof(void *), oldProtection, &oldProtection);
    if (!success)
    {
      debugPrintf("Failed to restore IAT entry protection 0x%p", IATentry);
      continue;
    }
  }
}

bool LibraryHooks::Detect(const char *identifier)
{
  bool ret = false;
  ForAllModules([&ret, identifier](const MODULEENTRY32 &me32) {
    if (GetProcAddress(me32.hModule, identifier) != NULL)
      ret = true;
  });
  return ret;
}

typedef int(WSAAPI *PFN_WSASTARTUP)(__in WORD wVersionRequested, __out LPWSADATA lpWSAData);
typedef int(WSAAPI *PFN_WSACLEANUP)();

typedef BOOL(WINAPI *PFN_CREATE_PROCESS_A)(LPCSTR lpApplicationName, LPSTR lpCommandLine,
  LPSECURITY_ATTRIBUTES lpProcessAttributes,
  LPSECURITY_ATTRIBUTES lpThreadAttributes,
  BOOL bInheritHandles, DWORD dwCreationFlags,
  LPVOID lpEnvironment, LPCSTR lpCurrentDirectory,
  LPSTARTUPINFOA lpStartupInfo,
  LPPROCESS_INFORMATION lpProcessInformation);

typedef BOOL(WINAPI *PFN_CREATE_PROCESS_W)(LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
  LPSECURITY_ATTRIBUTES lpProcessAttributes,
  LPSECURITY_ATTRIBUTES lpThreadAttributes,
  BOOL bInheritHandles, DWORD dwCreationFlags,
  LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
  LPSTARTUPINFOW lpStartupInfo,
  LPPROCESS_INFORMATION lpProcessInformation);

typedef BOOL(WINAPI *PFN_CREATE_PROCESS_AS_USER_A)(
  HANDLE hToken, LPCSTR lpApplicationName, LPSTR lpCommandLine,
  LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
  BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory,
  LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);

typedef BOOL(WINAPI *PFN_CREATE_PROCESS_AS_USER_W)(
  HANDLE hToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
  LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
  BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
  LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);

typedef BOOL(WINAPI *PFN_CREATE_PROCESS_WITH_LOGON_W)(LPCWSTR lpUsername, LPCWSTR lpDomain,
  LPCWSTR lpPassword, DWORD dwLogonFlags,
  LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
  DWORD dwCreationFlags, LPVOID lpEnvironment,
  LPCWSTR lpCurrentDirectory,
  LPSTARTUPINFOW lpStartupInfo,
  LPPROCESS_INFORMATION lpProcessInformation);

class SysHook : LibraryHook
{
public:
  SysHook()
  {
    // we start with a refcount of 1 because we initialise WSA ourselves for our own sockets.
    m_WSARefCount = 1;
  }

  void RegisterHooks()
  {
    debugPrintf("Registering Win32 system hooks");

    // register libraries that we care about. We don't need a callback when they are loaded
    LibraryHooks::RegisterLibraryHook("kernel32.dll", NULL);
    LibraryHooks::RegisterLibraryHook("advapi32.dll", NULL);
    LibraryHooks::RegisterLibraryHook("api-ms-win-core-processthreads-l1-1-0.dll", NULL);
    LibraryHooks::RegisterLibraryHook("api-ms-win-core-processthreads-l1-1-1.dll", NULL);
    LibraryHooks::RegisterLibraryHook("api-ms-win-core-processthreads-l1-1-2.dll", NULL);
    LibraryHooks::RegisterLibraryHook("ws2_32.dll", NULL);

    // we want to hook CreateProcess purely so that we can recursively insert our hooks (if we so
    // wish)
    CreateProcessA.Register("kernel32.dll", "CreateProcessA", CreateProcessA_hook);
    CreateProcessW.Register("kernel32.dll", "CreateProcessW", CreateProcessW_hook);

    CreateProcessAsUserA.Register("advapi32.dll", "CreateProcessAsUserA", CreateProcessAsUserA_hook);
    CreateProcessAsUserW.Register("advapi32.dll", "CreateProcessAsUserW", CreateProcessAsUserW_hook);

    CreateProcessWithLogonW.Register("advapi32.dll", "CreateProcessWithLogonW",
      CreateProcessWithLogonW_hook);

    // handle API set exports if they exist. These don't really exist so we don't have to worry
    // about double hooking, and also they call into the 'real' implementation in kernelbase.dll
    API110CreateProcessA.Register("api-ms-win-core-processthreads-l1-1-0.dll", "CreateProcessA",
      API110CreateProcessA_hook);
    API110CreateProcessW.Register("api-ms-win-core-processthreads-l1-1-0.dll", "CreateProcessW",
      API110CreateProcessW_hook);
    API110CreateProcessAsUserW.Register("api-ms-win-core-processthreads-l1-1-0.dll",
      "CreateProcessAsUserW", API110CreateProcessAsUserW_hook);

    API111CreateProcessA.Register("api-ms-win-core-processthreads-l1-1-1.dll", "CreateProcessA",
      API111CreateProcessA_hook);
    API111CreateProcessW.Register("api-ms-win-core-processthreads-l1-1-1.dll", "CreateProcessW",
      API111CreateProcessW_hook);
    API111CreateProcessAsUserW.Register("api-ms-win-core-processthreads-l1-1-0.dll",
      "CreateProcessAsUserW", API111CreateProcessAsUserW_hook);

    API112CreateProcessA.Register("api-ms-win-core-processthreads-l1-1-2.dll", "CreateProcessA",
      API112CreateProcessA_hook);
    API112CreateProcessW.Register("api-ms-win-core-processthreads-l1-1-2.dll", "CreateProcessW",
      API112CreateProcessW_hook);
    API112CreateProcessAsUserW.Register("api-ms-win-core-processthreads-l1-1-0.dll",
      "CreateProcessAsUserW", API112CreateProcessAsUserW_hook);

    WSAStartup.Register("ws2_32.dll", "WSAStartup", WSAStartup_hook);
    WSACleanup.Register("ws2_32.dll", "WSACleanup", WSACleanup_hook);

  }

private:
  static SysHook syshooks;
  int m_WSARefCount;

  HookedFunction<PFN_CREATE_PROCESS_A> CreateProcessA;
  HookedFunction<PFN_CREATE_PROCESS_W> CreateProcessW;

  HookedFunction<PFN_CREATE_PROCESS_A> API110CreateProcessA;
  HookedFunction<PFN_CREATE_PROCESS_W> API110CreateProcessW;
  HookedFunction<PFN_CREATE_PROCESS_A> API111CreateProcessA;
  HookedFunction<PFN_CREATE_PROCESS_W> API111CreateProcessW;
  HookedFunction<PFN_CREATE_PROCESS_A> API112CreateProcessA;
  HookedFunction<PFN_CREATE_PROCESS_W> API112CreateProcessW;

  HookedFunction<PFN_CREATE_PROCESS_AS_USER_A> CreateProcessAsUserA;
  HookedFunction<PFN_CREATE_PROCESS_AS_USER_W> CreateProcessAsUserW;

  HookedFunction<PFN_CREATE_PROCESS_AS_USER_W> API110CreateProcessAsUserW;
  HookedFunction<PFN_CREATE_PROCESS_AS_USER_W> API111CreateProcessAsUserW;
  HookedFunction<PFN_CREATE_PROCESS_AS_USER_W> API112CreateProcessAsUserW;

  HookedFunction<PFN_CREATE_PROCESS_WITH_LOGON_W> CreateProcessWithLogonW;

  HookedFunction<PFN_WSASTARTUP> WSAStartup;
  HookedFunction<PFN_WSACLEANUP> WSACleanup;

  static int WSAAPI WSAStartup_hook(WORD wVersionRequested, LPWSADATA lpWSAData)
  {
    int ret = syshooks.WSAStartup()(wVersionRequested, lpWSAData);

    // only increment the refcount if the function succeeded
    if (ret == 0)
      syshooks.m_WSARefCount++;

    return ret;
  }

  static int WSAAPI WSACleanup_hook()
  {
    // don't let the application murder our sockets with a mismatched WSACleanup() call
    if (syshooks.m_WSARefCount == 1)
    {
      debugPrintf("WSACleanup called with (to the application) no WSAStartup! Ignoring.");
      SetLastError(WSANOTINITIALISED);
      return SOCKET_ERROR;
    }

    // decrement refcount and call the real thing
    syshooks.m_WSARefCount--;
    return syshooks.WSACleanup()();
  }

  static BOOL WINAPI Hooked_CreateProcess(
    const char *entryPoint,
    std::function<BOOL(DWORD dwCreationFlags, LPPROCESS_INFORMATION lpProcessInformation)> realFunc,
    DWORD dwCreationFlags, bool inject, LPPROCESS_INFORMATION lpProcessInformation)
  {
      return realFunc(dwCreationFlags, lpProcessInformation);
  }

  static bool ShouldInject(LPCWSTR lpApplicationName, LPCWSTR lpCommandLine)
  {
    return false;
  }

  static bool ShouldInject(LPCSTR lpApplicationName, LPCSTR lpCommandLine)
  {
    return false;
  }

  static BOOL WINAPI CreateProcessA_hook(
    __in_opt LPCSTR lpApplicationName, __inout_opt LPSTR lpCommandLine,
    __in_opt LPSECURITY_ATTRIBUTES lpProcessAttributes,
    __in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes, __in BOOL bInheritHandles,
    __in DWORD dwCreationFlags, __in_opt LPVOID lpEnvironment, __in_opt LPCSTR lpCurrentDirectory,
    __in LPSTARTUPINFOA lpStartupInfo, __out LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessA",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.CreateProcessA()(lpApplicationName, lpCommandLine, lpProcessAttributes,
        lpThreadAttributes, bInheritHandles, flags,
        lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI CreateProcessW_hook(__in_opt LPCWSTR lpApplicationName,
    __inout_opt LPWSTR lpCommandLine,
    __in_opt LPSECURITY_ATTRIBUTES lpProcessAttributes,
    __in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes,
    __in BOOL bInheritHandles, __in DWORD dwCreationFlags,
    __in_opt LPVOID lpEnvironment,
    __in_opt LPCWSTR lpCurrentDirectory,
    __in LPSTARTUPINFOW lpStartupInfo,
    __out LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessW",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.CreateProcessW()(lpApplicationName, lpCommandLine, lpProcessAttributes,
        lpThreadAttributes, bInheritHandles, flags,
        lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI API110CreateProcessA_hook(
    __in_opt LPCSTR lpApplicationName, __inout_opt LPSTR lpCommandLine,
    __in_opt LPSECURITY_ATTRIBUTES lpProcessAttributes,
    __in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes, __in BOOL bInheritHandles,
    __in DWORD dwCreationFlags, __in_opt LPVOID lpEnvironment, __in_opt LPCSTR lpCurrentDirectory,
    __in LPSTARTUPINFOA lpStartupInfo, __out LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessA",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.API110CreateProcessA()(
        lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI API110CreateProcessW_hook(
    __in_opt LPCWSTR lpApplicationName, __inout_opt LPWSTR lpCommandLine,
    __in_opt LPSECURITY_ATTRIBUTES lpProcessAttributes,
    __in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes, __in BOOL bInheritHandles,
    __in DWORD dwCreationFlags, __in_opt LPVOID lpEnvironment, __in_opt LPCWSTR lpCurrentDirectory,
    __in LPSTARTUPINFOW lpStartupInfo, __out LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessW",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.API110CreateProcessW()(
        lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI API111CreateProcessA_hook(
    __in_opt LPCSTR lpApplicationName, __inout_opt LPSTR lpCommandLine,
    __in_opt LPSECURITY_ATTRIBUTES lpProcessAttributes,
    __in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes, __in BOOL bInheritHandles,
    __in DWORD dwCreationFlags, __in_opt LPVOID lpEnvironment, __in_opt LPCSTR lpCurrentDirectory,
    __in LPSTARTUPINFOA lpStartupInfo, __out LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessA",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.API111CreateProcessA()(
        lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI API111CreateProcessW_hook(
    __in_opt LPCWSTR lpApplicationName, __inout_opt LPWSTR lpCommandLine,
    __in_opt LPSECURITY_ATTRIBUTES lpProcessAttributes,
    __in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes, __in BOOL bInheritHandles,
    __in DWORD dwCreationFlags, __in_opt LPVOID lpEnvironment, __in_opt LPCWSTR lpCurrentDirectory,
    __in LPSTARTUPINFOW lpStartupInfo, __out LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessW",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.API111CreateProcessW()(
        lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI API112CreateProcessA_hook(
    __in_opt LPCSTR lpApplicationName, __inout_opt LPSTR lpCommandLine,
    __in_opt LPSECURITY_ATTRIBUTES lpProcessAttributes,
    __in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes, __in BOOL bInheritHandles,
    __in DWORD dwCreationFlags, __in_opt LPVOID lpEnvironment, __in_opt LPCSTR lpCurrentDirectory,
    __in LPSTARTUPINFOA lpStartupInfo, __out LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessA",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.API112CreateProcessA()(
        lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI API112CreateProcessW_hook(
    __in_opt LPCWSTR lpApplicationName, __inout_opt LPWSTR lpCommandLine,
    __in_opt LPSECURITY_ATTRIBUTES lpProcessAttributes,
    __in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes, __in BOOL bInheritHandles,
    __in DWORD dwCreationFlags, __in_opt LPVOID lpEnvironment, __in_opt LPCWSTR lpCurrentDirectory,
    __in LPSTARTUPINFOW lpStartupInfo, __out LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessW",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.API112CreateProcessW()(
        lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI CreateProcessAsUserA_hook(
    HANDLE hToken, LPCSTR lpApplicationName, LPSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory,
    LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessAsUserA",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.CreateProcessAsUserA()(
        hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI CreateProcessAsUserW_hook(
    HANDLE hToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessAsUserW",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.CreateProcessAsUserW()(
        hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI CreateProcessWithLogonW_hook(LPCWSTR lpUsername, LPCWSTR lpDomain,
    LPCWSTR lpPassword, DWORD dwLogonFlags,
    LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
    DWORD dwCreationFlags, LPVOID lpEnvironment,
    LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessAsUserW",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.CreateProcessWithLogonW()(
        lpUsername, lpDomain, lpPassword, dwLogonFlags, lpApplicationName, lpCommandLine,
        flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI API110CreateProcessAsUserW_hook(
    HANDLE hToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessAsUserW",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.API110CreateProcessAsUserW()(
        hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI API111CreateProcessAsUserW_hook(
    HANDLE hToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessAsUserW",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.API111CreateProcessAsUserW()(
        hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }

  static BOOL WINAPI API112CreateProcessAsUserW_hook(
    HANDLE hToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
  {
    return Hooked_CreateProcess(
      "CreateProcessAsUserW",
      [=](DWORD flags, LPPROCESS_INFORMATION pi) {
      return syshooks.API112CreateProcessAsUserW()(
        hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
        bInheritHandles, flags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, pi);
    },
      dwCreationFlags, ShouldInject(lpApplicationName, lpCommandLine), lpProcessInformation);
  }
};

SysHook SysHook::syshooks;
using std::wstring;

static BOOL add_hooks()
{
  wchar_t curFile[512];
  GetModuleFileNameW(NULL, curFile, 512);

  wstring f = strlower(wstring(curFile));

  // bail immediately if we're in a system process. We don't want to hook, log, anything -
  // this instance is being used for a shell extension.
  if (f.find(L"dllhost.exe") != wstring::npos || f.find(L"explorer.exe") != wstring::npos)
  {
    return TRUE;
  }

  LibraryHooks::RegisterHooks();

  return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  if (ul_reason_for_call == DLL_PROCESS_ATTACH)
  {
    BOOL ret = add_hooks();
    SetLastError(0);
    return ret;
  }

  return TRUE;
}