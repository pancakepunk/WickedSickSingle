#pragma once

#include "Math/MathInterface.h"

namespace WickedSick
{
  struct SceneConstants
  {
    SceneConstants();
    Vector4 ambientColor;
    Vector4 fogColor;
    float ambientIntensity;
    float fogNear;
    float fogFar;
    bool rotate;
  };
}
