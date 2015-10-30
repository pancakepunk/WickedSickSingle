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
    void SetLookAt(const Vector3& lookat);
    Vector3 GetLookAt();
  private:
    FogInfo draw_dist_info_;
    Vector3 look_at_;
  };
}
