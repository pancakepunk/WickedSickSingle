#pragma once
#include "Math/MathInterface.h"
namespace WickedSick
{
  struct TextureDesc
  {
    TextureDesc();
    Vector2i size;
    bool writable;
    bool target;
  };

  class Texture
  {
  public:
    Texture(const TextureDesc& desc);
    Texture(const std::string& source);
    Texture(const std::vector<unsigned char>& tex,
            const TextureDesc& desc);
    virtual ~Texture();
    virtual void* GetResourcePointer() = 0;
    virtual void* GetTexturePointer() = 0;
    virtual void Initialize() = 0;
    virtual std::string GetName() final;
    virtual std::string GetSource() final;
  protected:
    std::string name_;
    std::string source_;
    TextureDesc desc_;
    std::vector<unsigned char> tex_;
  };
}