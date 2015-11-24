#pragma once

#include "Math/MathInterface.h"

namespace WickedSick
{
  class Texture;

  struct RenderTargetDesc
  {
    Vector2i size;
  };

  class RenderTarget
  {
    public:
      RenderTarget(const RenderTargetDesc& desc);
      virtual ~RenderTarget();
      virtual void Initialize() = 0;
      virtual void* GetTargetPointer() = 0;
      Texture* GetTexture();
    protected:
      Texture* texture_;
      RenderTargetDesc desc_;
  };
}
