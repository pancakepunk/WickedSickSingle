#pragma once

#include "Core/CoreInterface.h"


namespace WickedSick
{
  struct FogInfo
  {
    float farDist;
    float nearDist;
    Vector4 fogColor;
  };
  class CameraComponent : public Component
  {
  public:
    CameraComponent();
    ~CameraComponent();
    void Initialize();
    void Update(float dt);
    void SetRotation(const Vector3& rot);
    Vector3 GetRotation();
    void SetLookAt(const Vector3& lookat);
    void Move(const Vector3i& movement);
    void Clone(Component* source) override;
    Vector3i GetMovement();
    Vector3 GetLookAt();
  private:
    FogInfo draw_dist_info_;
    Vector3i move_;
    Vector3 look_at_;
    Vector3 rotation_;
  };
}
