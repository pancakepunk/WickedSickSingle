#pragma once
#include "Architecture/RigidBody.h"

#include "Core/CoreInterface.h"
namespace WickedSick
{
  class RigidBody;
  class Collider;
  class PhysicsMesh;
  struct PhysicsMaterial;
  class PhysicsComponent : public Component
  {
    public:
    MetaDef;
    PhysicsComponent(bool isStatic = false,
                                      float mass = 1.0f,
                                      float inertiaTensor = 0.0f,
                                      Vector3 position = Vector3(),
                                      Vector3 centerofmass = Vector3(),
                                      float gravityScalar = 1.0f,
                                      float linearDamping = 0.01f,
                                      float angularDamping = 0.01f);
    ~PhysicsComponent();
    void clone(const PhysicsComponent& rhs);
    void SetPosition(Vector3 pos);
    void Reset();
    Vector3 GetPosition();
    Vector3 GetVelocity();
    void Initialize() override;
    void Update(float dt) override;
    std::set<PhysicsComponent*> GetCollisions();

    void AddCollider(Collider* collider);
    void AddCollider(RigidBody* base,
                                      PhysicsMaterial* material,
                                      PhysicsMesh* mesh,
                                      Matrix3 orientation_matrix = Matrix3(),
                                      float volume = 1.0f);
    RigidBody* GetRigidBody();

    private:
      RigidBody body_; //box in here
      
      
  };
}

