#include "GraphicsPrecompiled.h"
#include "GraphicsOptions.h"

namespace WickedSick
{
  GraphicsOptions::GraphicsOptions( bool     vs,
                                    const Vector2i& res,
                                    const Vector4&  clear,
                                    APIType::Enum  api) 
                                    : VSync(vs),
                                      Resolution(res),
                                      ClearColor(clear),
                                      Api(api)
  {

  }
}
