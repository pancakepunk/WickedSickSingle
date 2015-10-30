#pragma once
#include "D3D/DXIncludes.h"//I want to get rid of this but not sure how



namespace WickedSick
{
  struct Vector2i;
  struct Device;
  struct SwapChain
  {

    SwapChain();
    ~SwapChain();
    void Initialize(HWND& hWnd, const Vector2i& size);
    void CleanUp();

    Device* device;

    IDXGISwapChain* D3DSwapChain;
    DXGI_SWAP_CHAIN_DESC D3DDescription;
  };
}