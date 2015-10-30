#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace WickedSick
{
  struct Device
  {
    Device();
    ~Device();
    void CleanUp();
    ID3D11Device* D3DDevice;
    ID3D11DeviceContext* D3DContext;
  };
}