#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "GraphicsAPI.h"
#include "APIType.h"

namespace WickedSick
{
  GraphicsAPI::GraphicsAPI(APIType::Enum api) : options_(nullptr),
                                          api_(api)
  {
    
  }

  GraphicsAPI::~GraphicsAPI()
  {

  }

  void GraphicsAPI::AddRenderTarget(RenderTarget* target)
  {
    current_render_targets_.push_back(target);
  }

  void GraphicsAPI::ClearRenderTargets()
  {
    current_render_targets_.clear();
  }
}
