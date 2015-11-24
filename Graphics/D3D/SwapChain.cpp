#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "SwapChain.h"

#include "Graphics/D3D/Device.h"

#include "Graphics/General/GraphicsUtility.h"

#include "Math/MathInterface.h"

namespace WickedSick
{
  SwapChain::SwapChain() : D3DSwapChain(nullptr)
  {
    ZeroMemory(&D3DDescription, sizeof(D3DDescription));
    device = new Device();
  }

  SwapChain::~SwapChain()
  {
    CleanUp();
    delete device;
  }

  void SwapChain::Initialize(HWND& hWnd, const Vector2i& size)
  {

    D3DDescription.BufferCount = 1;                                 // # of backbuffers
    D3DDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 32 bit color
    D3DDescription.BufferDesc.Width = size.x;
    D3DDescription.BufferDesc.Height = size.y;
    D3DDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // swap chain use
    D3DDescription.OutputWindow = hWnd;                             // output window
    D3DDescription.SampleDesc.Count = 1;                            // auto anti aliasing, tweakable
    D3DDescription.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevels[] = 
    {
      D3D_FEATURE_LEVEL_11_0
    };

    D3D_FEATURE_LEVEL supportedLevel;

    auto error = GetLastError();
    DxError(D3D11CreateDeviceAndSwapChain(nullptr,                  // adapter pointer. nullptr means default
                                          D3D_DRIVER_TYPE_HARDWARE, // driver type. we want hardware.
                                          nullptr,                  // software rasterizer. used if previous parameter is D3D_DRIVER_TYPE_SOFTWARE
                                          D3D11_CREATE_DEVICE_DEBUG,                        // device flags. got none atm.
                                          featureLevels,                  // feature level array goes here. using default array.
                                          ARRAYSIZE(featureLevels),                        // number of elements in the feature level array
                                          D3D11_SDK_VERSION,        // current sdk version (we're in 11)
                                          &D3DDescription,          // description pointer (will be filled)
                                          &D3DSwapChain,            // swap chain (will be filled)
                                          &device->D3DDevice,       // device (will be filled)
                                          &supportedLevel,                  // feature level pointer. we don't care atm.
                                          &device->D3DContext));     // device context (will be filled)
    
    ShowWindow(hWnd, SW_SHOW);
  }

  void SwapChain::CleanUp()
  {
    D3DSwapChain->Release();
    device->CleanUp();
  }

  

}
