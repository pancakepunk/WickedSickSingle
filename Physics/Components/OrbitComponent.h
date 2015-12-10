#pragma once
#include "Core/CoreInterface.h"
#include "PhysicsComponent.h"



namespace WickedSick
{
  class OrbitComponent : public Component
  {
  public:
    OrbitComponent(Vector3 initialVec = Vector3(1.0f, 1.0f, 1.0f), 
                                  Vector3 target = Vector3(),
                                  float targetDist = 20.0f,
                                  float force = 10.0f);
    ~OrbitComponent();

    void Initialize() override;
    void Update(float dt) override;
    void Clone(Component* source) override;

    void Release();
    void Target();
  private:
    bool targeting_;
    float force_;
    float target_dist_;
    Vector3 current_dir_;
    Vector3 target_;

  };
}
