#include "GraphicsPrecompiled.h"
#include "Texture.h"

namespace WickedSick
{
  Texture::Texture(const std::string& source) : source_(source)
  {
    //friendly name
    //npos is -1 so this works
    int start = source_.find_last_of("/\\") + 1;
    int len = (source_.find_last_of(".")) - start;
    name_ = source.substr(start, len);

  }

  Texture::~Texture()
  {

  }

  std::string Texture::GetName()
  {
    return name_;
  }

  std::string Texture::GetSource()
  {
    return source_;
  }
}
