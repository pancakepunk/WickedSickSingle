
#include "PhysicsPrecompiled.h"
#include "Contact.h"
namespace WickedSick
{
  Contact::Contact()
    : penetration_depth_(0.0f)
    , normal_impulse_sum_(0.0f)
    , tangent_impulse_sum_1_(0.0f)
    , tangent_impulse_sum_2_(0.0f)
  {
    
  }

  void Contact::find_basis()
  {
    // find least axis of least significant component
    const float absX = std::fabs(normal_.x);
    const float absY = std::fabs(normal_.y);
    const float absZ = std::fabs(normal_.z);
    Vector3 axis(0.0f, 0.0f, 0.0f);
    if (absX < absY)
    {
      if (absX < absZ)
      {
        axis.x = 1.0f; // X < Y < Z, X < Z < Y
      }
      else
      {
        axis.z = 1.0f; // Z < X < Y
      }
    }
    else
    {
      if (absY < absZ)
      {
        axis.y = 1.0f; // Y < X < Z, Y < Z X
      }
      else
      {
        axis.z = 1.0f; // Z < Y < X
      }
    }
    // compute tangents
    tangent_1_ = normal_.Cross(axis);
    tangent_1_.Normalize();
    tangent_2_ = normal_.Cross(tangent_1_);
    tangent_2_.Normalize();
  }
}
