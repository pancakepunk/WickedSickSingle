#pragma once

#include "Texture.h"
#include "Math/MathInterface.h"
#include "Graphics/D3D/WICTextureLoader.h"
namespace WickedSick
{

  class DxTexture : public Texture
  {
  public:
    DxTexture(const TextureDesc& desc);
    DxTexture(const std::string& texName);
    DxTexture(const std::vector<unsigned char> & texName,
              const TextureDesc& desc);
    ~DxTexture();
    void Initialize();
    void Cleanup();
    void* GetResourcePointer();
    void* GetTexturePointer();
  private:
    ID3D11Texture2D*          texture_2d_;
    ID3D11ShaderResourceView* texture_view_;
  };
}
