#include "Precompiled.h"
#include "RandomClass.h"
#include "Core/CoreInterface.h"
namespace WickedSick
{

  Random::Random() : generator_(device_())
  {
    
    generator_.seed((unsigned) FrameController::CurrentTime() * 1000u);
    normal_distribution_ = std::uniform_real_distribution<float>(-1.0f, 1.0f);
  }
  float Random::Gen()
  {
    return normal_distribution_(generator_);
  }
}
