#pragma once
#include "Math/MathInterface.h"
#include "Meta/MetaInterface.h"
namespace WickedSick
{

  struct ParticleDescription
  {
    MetaDef;
    ParticleDescription();
    ParticleDescription(const Vector3 & pPosition,
                        const Vector3 & vVelocity,
                        const Vector4 & cColor,
                        const Vector3 & sScale,
                        float rRotation,
                        float lLifetime);
    //init data
    Vector3 Position;
    Vector3 Velocity;
    Vector4 Color;
    Vector3 Scale;
    Vector3 PrevPos;//runtime data
    float Rotation;
    float Lifetime;
    float TimePassed;
    float temp;

    ParticleDescription operator+(const ParticleDescription& rhs);
    ParticleDescription operator-(const ParticleDescription& rhs);
    ParticleDescription operator-();
    ParticleDescription operator*(float scalar);

  };
}
