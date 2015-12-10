#pragma once


#include "Resolution/Constraint.h"
#include "Resolution/ConstraintList.h"
#include "MotionState.h"

namespace WickedSick
{
  class Collider;
  class PhysicsComponent;
  class RigidBody
  {
  public:
    MetaDef;
    RigidBody( PhysicsComponent* base,
                                bool isStatic = false,
                                float mass = 0.0f,
                                float inertiaTensor = 0.0f,
                                Vector3 position = Vector3(),
                                Vector3 centerofmass = Vector3(),
                                float gravityScalar = 1.0f,
                                float linearDamping = 0.01f,
                                float angularDamping = 0.01f);

    void Integrate(const float& dt);
    void AddForce(Vector3 force);
    void AddTorque(Vector3 torque);
      




    void clone(const RigidBody& rhs);

    void SetPosition(Vector3 pos);
    void SetOrientation(Vector3 pos);
    void SetGravityScalar(float scalar);
    void SetVelocity(Vector3 vel);
    void SetAngularVelocity(Vector3 vel);
    void SetBase(PhysicsComponent* base);
    void SetStatic(bool isStatic);



    bool IsStatic();
    float GetGravityScalar();
    MotionState& GetState();
    MotionState& GetPreviousState();
    std::vector<Collider*>& GetColliders();
    ConstraintList& GetConstraints();
      //std::vector<Collider*> GetCollisionsWith(const std::string& collider);
    PhysicsComponent* GetBase();

    float GetLinearDamping() const;
    float GetAngularDamping() const;

    void SetLinearDamping(float newDamp);
    void SetAngularDamping(float newDamp);

  private:
    void stop_movement();
    void reset_orientation();
    void render_debug();

    float                     gravity_scalar_;
    float                     linear_damping_;
    float                     angular_damping_;
    MotionState               state_;
    MotionState               previous_state_;
    std::vector<Collider*>    colliders_;
    ConstraintList            constraints_;
    bool                      static_;
    PhysicsComponent*         base_;

    bool draw_debug_;

  };
}                                                                
                