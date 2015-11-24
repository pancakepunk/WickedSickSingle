#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "Texture.h"
#include "Math/MathInterface.h"

namespace WickedSick
{
  Texture::Texture(const TextureDesc& desc)
  : desc_(desc)
  {
  }
  
  Texture::Texture(const std::string& source)
  : source_(source)
  {
    //friendly name
    //npos is -1 so this works
    int start = source_.find_last_of("/\\") + 1;
    int len = (source_.find_last_of(".")) - start;
    name_ = source.substr(start, len);
    memset(&desc_, 0, sizeof(desc_));
  }

  Texture::Texture(const std::vector<unsigned char>& tex, 
                   const TextureDesc& desc)
  : tex_(tex),
    desc_(desc)
  {
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

  TextureDesc::TextureDesc() 
  : writable(false),
    target(false)
  {
  }
}
