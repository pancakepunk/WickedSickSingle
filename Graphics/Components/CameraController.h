#pragma once

#include "Core/CoreInterface.h"
#include "Math/MathInterface.h"


namespace WickedSick
{

  class CameraController : public Component
  {
  public:
    CameraController();
    ~CameraController();
    void Initialize();
    void Update(float dt);
  private:
    Vector2i last_mouse_pos_;
  };
}
