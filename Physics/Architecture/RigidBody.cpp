#include "Precompiled.h"

#include "PhysicsPrecompiled.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Components\PhysicsComponent.h"

#include "PhysicsScene.h"
//#include "Octree.h"

#include "System/Physics.h"

//debug draw
#include "PhysicsMesh.h"
//


namespace WickedSick
{

  RigidBody::RigidBody( PhysicsComponent * base,
                                       bool isStatic,
                                       float mass,
                                       float inertiaTensor,
                                       Vector3 position,
                                       Vector3 centerofmass,
                                       float gravityScalar,
                                       float linearDamping,
                                       float angularDamping)
                                        : state_( mass,
                                                  inertiaTensor,
                                                  position,
                                                  centerofmass),
                                          static_(isStatic),
                                          gravity_scalar_(gravityScalar),
                                          angular_damping_(angularDamping),
                                          linear_damping_(linearDamping),
                                          base_(base),
                                          draw_debug_(false)
  {
    previous_state_ = state_;
  }

  bool RigidBody::IsStatic() 
  { 
    return static_; 
  }

  void RigidBody::clone(const RigidBody& rhs)
  {
    draw_debug_ = rhs.draw_debug_;
    gravity_scalar_ = rhs.gravity_scalar_;
    linear_damping_ = rhs.linear_damping_;
    angular_damping_ = rhs.angular_damping_;
    state_ = rhs.state_;
    previous_state_ = rhs.previous_state_;
    static_ = rhs.static_;
    for(auto it : rhs.colliders_)
    {
      colliders_.push_back(new Collider(this));
      colliders_.back()->clone(*it);
    }
  }

  void RigidBody::SetPosition(Vector3 pos) 
  { 
    //stop_movement();
    //reset_orientation();
    state_.SetPosition(pos); 
    if(static_)
    {
      previous_state_.SetPosition(pos);
    }
  }

  void RigidBody::SetOrientation(Vector3 pos)
  {
    //stop_movement();
    //reset_orientation();
    state_.Orientation = Vector4(pos, 0.0f);
    if(static_)
    {
      previous_state_.Orientation = state_.Orientation;
    }
  }

  void RigidBody::SetGravityScalar(float scalar)
  {
    gravity_scalar_ = scalar;
  }

  float RigidBody::GetGravityScalar()
  {
    return gravity_scalar_;
  }

  MotionState & RigidBody::GetState() 
  { 
    return state_; 
  }

  MotionState& RigidBody::GetPreviousState() 
  { 
    return previous_state_; 
  }

  std::vector<Collider*>& RigidBody::GetColliders() 
  { 
    return colliders_; 
  }

  ConstraintList& RigidBody::GetConstraints() 
  { 
    return constraints_; 
  }

  //std::vector<Collider*> GetCollisionsWith(const std::string& collider);

  PhysicsComponent * RigidBody::GetBase() 
  { 
    return base_; 
  }

  void RigidBody::SetVelocity(Vector3 vel)
  {
    previous_state_.Velocity = vel;
    state_.Velocity = vel;
  }

  void RigidBody::SetAngularVelocity(Vector3 vel)
  {
    previous_state_.AngularVelocity = vel;
    state_.AngularVelocity = vel;
  }

  void RigidBody::SetBase(PhysicsComponent * base) 
  { 
    base_ = base; 
  }

  void RigidBody::SetStatic(bool isStatic) 
  { 
    static_ = isStatic; 
  }

  void RigidBody::AddForce(Vector3 force)
  {
    state_.Force += force;
  }

  void RigidBody::stop_movement()
  {
    state_.StopMovement();
  }

  void RigidBody::reset_orientation()
  {
    state_.ResetOrientation();
  }

  void RigidBody::render_debug()
  {

  }

  void RigidBody::AddTorque(Vector3 torque)
  {
    state_.Torque += torque;
  }

  void RigidBody::Integrate(const float& dt)
  {

    if(draw_debug_)
    {
      render_debug();
    }

    previous_state_ = state_;
    if(!static_)
    {
      state_.Position += state_.Velocity * dt;
      //state_.CenterOfMass += state_.Velocity * dt;
      state_.Orientation += state_.AngularVelocity * dt;

      //gravity here
      state_.Velocity += ((Physics::Gravity * gravity_scalar_) + (state_.Force * state_.InverseMass)) * dt;
      state_.AngularVelocity += (state_.Torque * state_.InverseInertiaTensor) * dt;

      //damping!
      state_.Velocity *= 1.0f / (1.0f + dt * linear_damping_);
      state_.AngularVelocity *= 1.0f / (1.0f + dt * angular_damping_);

      state_.Force = Vector3();
      state_.Torque = Vector3();
    }

    if(state_.Position != previous_state_.Position)
    {

      //todo: update broadphase
    }
  }

}

RegisterType(WickedSick, RigidBody)
RegisterMember(static_);
}

