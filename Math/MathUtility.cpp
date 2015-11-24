#include "Precompiled.h"
#include "MathPrecompiled.h"
#include "MathUtility.h"
#include "Vector/Vector4.h"

namespace WickedSick
{
  float AngleBetween(const Vector3& left, const Vector3& right)
  {
    return acos(left.GetNormalized().Dot(right.GetNormalized()));
  }

  Vector3 RotateAround(const Vector3 & axis, float amount, const Vector3 & vectorToRotate)
  {
    Matrix4 identity;
    identity.Identity();
    Matrix4 first   = identity * cos(amount);
    Matrix4 second  = TensorProductMatrix(axis) * (1 - cos(amount));
    Matrix4 third   = CrossProductMatrix(axis) * sin(amount);
    Vector4 endVal = (first + second + third) * vectorToRotate;
    return Vector3(endVal.x, endVal.y, endVal.z);
  }

  Matrix4 CrossProductMatrix(const Vector3 & n)
  {
    return Matrix4(0.0f, -n.z, n.y, 0.0f,
                   n.z, 0.0f, -n.x, 0.0f,
                   -n.y, n.x, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
  }

  Matrix4 TensorProductMatrix(const Vector3 & n)
  {
    return Matrix4(n.x * n.x, n.x * n.y, n.x * n.z, 0.0f,
                   n.y * n.x, n.y * n.y, n.y * n.z, 0.0f,
                   n.z * n.x, n.z * n.y, n.z * n.z, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
  }
}

