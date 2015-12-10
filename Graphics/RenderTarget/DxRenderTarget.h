#pragma once
#include "Graphics/RenderTarget/RenderTarget.h"
#include "Graphics/D3D/DXIncludes.h"
namespace WickedSick
{
  class DxRenderTarget : public RenderTarget
  {
    public:
      DxRenderTarget(const RenderTargetDesc& desc);
      ~DxRenderTarget();
      void Initialize(void* const& texture = nullptr) override;
      void* GetTargetPointer() override;
    private:
      ID3D11RenderTargetView* render_target_;
  };
}