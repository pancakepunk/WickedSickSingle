#pragma once

namespace WickedSick
{

  struct MotionState
  {
    MotionState(float mass = 0.0f,
                float inertiaTensor = 0.0f,
                Vector3 position = Vector3(),
                Vector3 centerofmass = Vector3(),
                Vector4 orientation = Vector4(),
                Vector3 velocity = Vector3(),
                Vector3 angularVelocity = Vector3());
    float Mass;
    float InverseMass;
    float InertiaTensor;
    float InverseInertiaTensor;

    Vector3 Position;
    Vector3 CenterOfMass;
    Vector4 Orientation;

    Vector3 Velocity;
    Vector3 AngularVelocity;

    Vector3 Force;
    Vector3 Torque;

    //abrupt stop.
    //ugly.
    void StopMovement();
    void ResetOrientation();
    void SetPosition(Vector3 pos) 
    { 
      Vector3 translate(pos - Position); 
      Position += translate;
      //CenterOfMass += translate;
    }

  };
}

