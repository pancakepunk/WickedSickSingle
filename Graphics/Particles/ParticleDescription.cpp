#include "Precompiled.h"
#include "ParticleDescription.h"

namespace WickedSick
{
  ParticleDescription::ParticleDescription()
  : Position(),
    Velocity(),
    Color(),
    Scale(),
    Rotation(0.0f),
    Lifetime(0.0f),
    PrevPos(),
    TimePassed(0.0f)
  {
  }

  ParticleDescription::ParticleDescription(const Vector4 & pPosition,
                                         const Vector4 & vVelocity,
                                         const Vector4 & cColor,
                                         const Vector4 & sScale,
                                         float rRotation,
                                         float lLifetime)
  : Position(pPosition),
    Velocity(vVelocity),
    Color(cColor),
    Scale(sScale),
    Rotation(rRotation),
    PrevPos(pPosition),
    Lifetime(lLifetime),
    TimePassed(0.0f)
  {

  }

  ParticleDescription ParticleDescription::operator+(const ParticleDescription & rhs)
  {
    return ParticleDescription(Position + rhs.Position, Velocity + rhs.Velocity, Color + rhs.Color, Scale + rhs.Scale, Rotation + rhs.Rotation, Lifetime);
  }

  ParticleDescription ParticleDescription::operator-(const ParticleDescription & rhs)
  {
    return ParticleDescription(Position - rhs.Position, Velocity - rhs.Velocity, Color - rhs.Color, Scale - rhs.Scale, Rotation - rhs.Rotation, Lifetime);
  }

  ParticleDescription ParticleDescription::operator-()
  {
    return ParticleDescription(-Position, -Velocity, -Color, -Scale, -Rotation, Lifetime);
  }

  ParticleDescription ParticleDescription::operator*(float scalar)
  {
    return ParticleDescription(Position * scalar, Velocity * scalar, Color * scalar, Scale * scalar, Rotation * scalar, Lifetime);
  }
}

RegisterType(WickedSick, ParticleDescription)
RegisterMember(Position);
RegisterMember(Velocity);
RegisterMember(Color);
RegisterMember(Scale);
RegisterMember(Rotation);
RegisterMember(Lifetime);
}
