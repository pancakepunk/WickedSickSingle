#pragma once

#include "Math/MathInterface.h"


namespace WickedSick
{
  class CameraComponent;
  class Camera
  {
  public:
    Camera();
    ~Camera();

    void SetPosition(float x, float y, float z);
    void SetPosition(const Vector3& pos);
    void SetRotation(float x, float y, float z);
    void SetRotation(const Vector3& rot);

    Vector3 GetPosition();
    Vector3 GetRotation();
    Vector3 GetLookAt();

    void Orient(float dt);
    
    void SetSource(CameraComponent* source);

    Matrix4 GetViewMatrix();

    void Dirty();
    void Clean();
    bool IsDirty();

  private:
    CameraComponent* source_;
    Vector3 rot_;
    Vector3 pos_;
    Vector3 look_at_;

    Matrix4 view_;

    bool dirty_;

  };
}