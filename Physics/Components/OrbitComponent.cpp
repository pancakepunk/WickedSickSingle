#include "Precompiled.h"
#include "PhysicsPrecompiled.h"
#include "OrbitComponent.h"

namespace WickedSick
{
  OrbitComponent::OrbitComponent(Vector3 initialVec,
                                                Vector3 target, 
                                                float targetDist,
                                                float force)
                                                : targeting_(true),
                                                  force_(force),
                                                  target_dist_(targetDist),
                                                  current_dir_(initialVec.GetNormalized()),
                                                  target_(target),
                                                  Component(CT_OrbitComponent)
  {
  }

  OrbitComponent::~OrbitComponent()
  {
  }

  void OrbitComponent::Initialize()
  {
  }

  void OrbitComponent::Update(float dt)
  {
    if (targeting_)
    {
      Transform* tr = (Transform*)GetSibling(CT_Transform);
      Vector3 forceVec = current_dir_;
      Vector3 toTarget = target_ - tr->GetPosition();
      float bias = toTarget.Length() / target_dist_;
      forceVec += toTarget.GetNormalized() * bias;
      PhysicsComponent* pComp = (PhysicsComponent*)GetSibling(CT_PhysicsComponent);
      if (pComp)
      {
        pComp->GetRigidBody()->AddForce(forceVec);
        current_dir_ = pComp->GetRigidBody()->GetState().Velocity.GetNormalized();
        pComp->GetRigidBody()->GetState().Velocity = pComp->GetRigidBody()->GetState().Velocity.GetNormalized() * 2.0f;
      }
    }
  }


  void OrbitComponent::Release()
  {
    targeting_ = false;
  }

  void OrbitComponent::Target()
  {
    targeting_ = true;
  }
}

