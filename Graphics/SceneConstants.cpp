#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "SceneConstants.h"

namespace WickedSick
{
  SceneConstants::SceneConstants() 
  : ambientIntensity(0.1f),
    fogNear(500.0f),
    fogFar(1000.0f),
    fogColor(0.1f, 0.1f, 0.1f, 1.0f)
  {
  }
}
