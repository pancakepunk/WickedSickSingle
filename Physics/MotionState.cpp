
#include "PhysicsPrecompiled.h"
#include "MotionState.h"

namespace WickedSick
{
  MotionState::MotionState(float mass,
                           float inertiaTensor,
                           Vector3 position,
                           Vector3 centerofmass,
                           Vector4 orientation,
                           Vector3 velocity,
                           Vector3 angularVelocity) : Position(position),
                                                      CenterOfMass(centerofmass),
                                                      Orientation(orientation),
                                                      Velocity(velocity),
                                                      AngularVelocity(angularVelocity),
                                                      Mass(mass),
                                                      InertiaTensor(inertiaTensor)
  {
    if(Mass > 0.0f)
    {
      InverseMass = 1.0f/Mass;
    }
    else
    {
      InverseMass = 0.0f;
    }
    if(InertiaTensor > 0.0f)
    {
      InverseInertiaTensor = 1.0f/InertiaTensor;
    }
    else
    {
      InverseInertiaTensor = 0.0f;
    }
  }



  void MotionState::ResetOrientation()
  {
    Orientation.Zero();
  }

  void MotionState::StopMovement()
  {
    Force.Zero();
    Velocity.Zero();
    AngularVelocity.Zero();
  }
}
