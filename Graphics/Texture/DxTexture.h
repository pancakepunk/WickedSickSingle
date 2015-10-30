#pragma once
#include <string>
#include "Texture.h"
#include "Graphics/D3D/WICTextureLoader.h"
namespace WickedSick
{

  class DxTexture : public Texture
  {
  public:
    DxTexture(const std::string& texName);
    ~DxTexture();
    void Initialize();
    void Cleanup();
    void* GetTexturePointer();
  private:
    ID3D11ShaderResourceView* texture_view_;
  };
}
