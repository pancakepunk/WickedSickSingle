#include "Precompiled.h"
#include "RenderTarget.h"

#include "Graphics/GraphicsInterface.h"
#include "Core/CoreInterface.h"
namespace WickedSick
{
  RenderTarget::RenderTarget(const RenderTargetDesc& desc)
  : desc_(desc),
    texture_(nullptr)
  {
    
  }

  RenderTarget::~RenderTarget()
  {
  }

  Texture* RenderTarget::GetTexture()
  {
    return texture_;
  }

}
