#include <d3d11.h>
#include <tchar.h>
#include <vector>
#include <string>

static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;

HRESULT CreateDeviceD3D(HWND hWnd)
{
  // Setup swap chain
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 2;
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  UINT createDeviceFlags = 0;
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  D3D_FEATURE_LEVEL featureLevel;
  const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };
  if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 1, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
    return E_FAIL;

  return S_OK;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

  switch (msg)
  {
  case WM_SIZE:
    if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
    {
    }
    return 0;
  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
      return 0;
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND g_window;
typedef void(*Event)(void);
extern std::vector<std::pair<std::string, Event>> g_events;

int main(int, char**)
{
  WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("Example"), NULL };
  RegisterClassEx(&wc);
  HWND hwnd = CreateWindow(_T("Example"), _T("Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);
  ShowWindow(hwnd, SW_SHOWDEFAULT);
  UpdateWindow(hwnd);
	g_window = hwnd;
  /*if (CreateDeviceD3D(hwnd) < 0)
  {
    return 1;
  }*/

	for (auto const &item : g_events)
	{
		item.second();
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
	}

  MSG msg;
  ZeroMemory(&msg, sizeof(msg));
  bool oneshot = false;
  while (msg.message != WM_QUIT)
  {
		for (auto const &item : g_events)
		{
			if (
				item.first.find("Draw") != std::string::npos
				|| item.first.find("Present") != std::string::npos
				|| item.first.find("PS") != std::string::npos
				|| item.first.find("VS") != std::string::npos
				|| item.first.find("HS") != std::string::npos
				|| item.first.find("DS") != std::string::npos
				|| item.first.find("GS") != std::string::npos
				|| item.first.find("OM") != std::string::npos
				|| item.first.find("RS") != std::string::npos
				|| item.first.find("IA") != std::string::npos
				|| item.first.find("Map") != std::string::npos
				|| item.first.find("Unmap") != std::string::npos
				|| item.first.find("Clear") != std::string::npos
				)
				item.second();
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
		}
    // Poll and handle messages (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      continue;
    }
  }
  DestroyWindow(hwnd);
  UnregisterClass(_T("ImGui Example"), wc.hInstance);
	return 0;
}