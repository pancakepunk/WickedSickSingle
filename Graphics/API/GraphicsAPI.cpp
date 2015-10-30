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
}
