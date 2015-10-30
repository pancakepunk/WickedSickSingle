#pragma once
#include <string>
namespace WickedSick
{
  class Texture
  {
  public:
    Texture(const std::string& source);
    virtual ~Texture();
    virtual void* GetTexturePointer() = 0;
    virtual void Initialize() = 0;
    virtual std::string GetName() final;
    virtual std::string GetSource() final;
  protected:
    std::string name_;
    std::string source_;
  };
}