#include "GraphicsPrecompiled.h"
#include "Device.h"

#include "D3D/DXIncludes.h"

namespace WickedSick
{
  Device::Device() :  D3DDevice(nullptr),
                      D3DContext(nullptr)
  {

  }

  Device::~Device()
  {
    CleanUp();
  }

  void Device::CleanUp()
  {
    if(D3DContext){D3DContext->Release();}
    if(D3DDevice) {D3DDevice->Release();  }
  }
}