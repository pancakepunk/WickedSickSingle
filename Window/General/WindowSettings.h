#pragma once
#include "Math/MathInterface.h"

namespace WickedSick
{
  enum WindowType;
  struct WindowSettings
  {
    WindowSettings();
    Vector2i resolution;
    short bitColor;
    WindowType windowType;
    short refreshRate;
  };
}
