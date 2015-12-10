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
    void Initialize() override;
    void Update(float dt) override;
    void Clone(Component* source) override;

  private:
    Vector3 rotation_;
    Vector2i last_mouse_pos_;
  };
}
