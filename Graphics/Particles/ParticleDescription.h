#pragma once
#include "Math/MathInterface.h"
#include "Meta/MetaInterface.h"
namespace WickedSick
{

  struct ParticleDescription
  {
    MetaDef;
    ParticleDescription();
    ParticleDescription(const Vector4 & pPosition,
                       const Vector4 & vVelocity,
                       const Vector4 & cColor,
                       const Vector4 & sScale,
                       float rRotation,
                       float lLifetime);
    //init data
    Vector4 Position;
    Vector4 Velocity;
    Vector4 Color;
    Vector4 Scale;
    Vector4 PrevPos;//runtime data
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
