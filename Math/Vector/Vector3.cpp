#include "MathPrecompiled.h"
#include "Vector3.h"

#include "Vector/Vector2.h"
#include "Vector/Vector3i.h"

#include "Matrix/Matrix4.h"
#include "Matrix/Matrix3.h"


#include "Math/MathUtility.h"
namespace WickedSick
{
  Vector3::Vector3(void) : 
    x(0.0f), y(0.0f), z(0.0f)
  {
  }

  Vector3::Vector3(const Vector2& rhs, float zz) : 
    x(rhs.x), y(rhs.y), z(zz)
  {
  }

  Vector3::Vector3(const Vector3& rhs) : 
    x(rhs.x), y(rhs.y), z(rhs.z)
  {
    
  }

  Vector3::Vector3(float xx, float yy, float zz) : 
    x(xx), y(yy), z(zz)
  {
  }

  Vector3  & Vector3::operator=(const Vector3& rhs)
  {
    if(this != &rhs)
    {
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
    }
    return *this;
  }

  Vector3 Vector3::operator-(void) const
  {
    return Vector3(-x, -y, -z);
  }

  Vector3 Vector3::operator+(const Vector3& rhs) const
  {
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
  }

  Vector3 Vector3::operator-(const Vector3& rhs) const
  {
    return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
  }

  Vector3 Vector3::operator*(const float rhs) const
  {
    return Vector3(x * rhs, y * rhs, z * rhs);
  }

  Vector3 Vector3::operator/(const float rhs) const
  {
    return Vector3(x / rhs, y / rhs, z / rhs);
  }


  Vector3  & Vector3::operator+=(const Vector3& rhs)
  {
    x += rhs.x;
    y += rhs.y; 
    z += rhs.z;
    return *this;
  }

  Vector3  & Vector3::operator-=(const Vector3& rhs)
  {
    x -= rhs.x;
    y -= rhs.y; 
    z -= rhs.z;
    return *this;
  }

  Vector3  & Vector3::operator*=(const float rhs)
  {
    x *= rhs;
    y *= rhs; 
    z *= rhs;
    return *this;
  }

  Vector3  & Vector3::operator/=(const float rhs)
  {
    x /= rhs;
    y /= rhs; 
    z /= rhs;
    return *this;
  }

  bool Vector3::operator==(const Vector3& rhs) const
  {
    return  abs(x - rhs.x) <= EPSILON &&
            abs(y - rhs.y) <= EPSILON &&
            abs(z - rhs.z) <= EPSILON;
  }

  bool Vector3::operator!=(const Vector3& rhs) const
  {
    return  abs(x - rhs.x) > EPSILON ||
            abs(y - rhs.y) > EPSILON ||
            abs(z - rhs.z) > EPSILON;
  }

  Vector3 Vector3::operator*(const Matrix4& rhs) const
  {
    float w = x * rhs.m03 + y * rhs.m13 + z * rhs.m23 + rhs.m33;
  
    // guard against divide by zero
    float invW = 0;
    if( w ) invW = 1.0f / w;
  
    return Vector3(x * rhs.m00 + y * rhs.m10 + z * rhs.m20 + rhs.m30,
                   x * rhs.m01 + y * rhs.m11 + z * rhs.m21 + rhs.m31,
                   x * rhs.m02 + y * rhs.m12 + z * rhs.m22 + rhs.m32) * invW;
  }
  
  Vector3 Vector3::operator*(const Matrix3& rhs) const
  {
    return Vector3( x * rhs.m00 + y * rhs.m10 + z * rhs.m20,
                    x * rhs.m01 + y * rhs.m11 + z * rhs.m21,
                    x * rhs.m02 + y * rhs.m12 + z * rhs.m22);
  }

  float Vector3::Length(void) const
  {
    return sqrt(LengthSq());
  }


  float Vector3::LengthSq(void) const
  {
    return (x * x) + (y * y) + (z * z);
  }

  void Vector3::Negate(void)
  {
    x = -x; y = -y; z = -z;
  }

  Vector3 Vector3::GetNormalized() const
  {
    float len = Length();
    if(len == 0)
    {
      return Vector3();
    }
    return Vector3(x / len, y / len, z / len);
  }

  void Vector3::Normalize()
  {
    float len = Length();
    if(len == 0.0f)
    {
      //Warn("error: normalizing a zero vector");
      Zero();
      return;
    }

    x /= len;
    y /= len;
    z /= len;
  }

  float Vector3::Dot(const Vector3& rhs) const
  {
    return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
  }

  Vector3 Vector3::Cross(const Vector3& rhs) const
  {
    return Vector3( y * rhs.z - z * rhs.y,
                    z * rhs.x - x * rhs.z,
                    x * rhs.y - y * rhs.x);
  }

  bool Vector3::IsZero(void)
  {
    if(x == 0.0f && y == 0.0f && z == 0.0f) return true;
    else                                    return false;
  }

  void Vector3::Zero(void)
  {
    x = y = z = 0.0f;
  }

  void Vector3::Print(void) const
  {
    ConsolePrint(std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z));
  }

  Vector3 Vector3::Min(const Vector3 & rhs)
  {
    return Vector3(std::min(x, rhs.x), std::min(y, rhs.y), std::min(z, rhs.z));
  }

  Vector3 Vector3::Max(const Vector3 & rhs)
  {
    return Vector3(std::max(x, rhs.x), std::max(y, rhs.y), std::max(z, rhs.z));
  }


  Vector3i Vector3::to_i()
  {
    return Vector3i(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
  }

  std::string Vector3::to_string()
  {
    return std::to_string(x) + std::to_string(y) + std::to_string(z);
  }

  float  & Vector3::operator [](int index)
  {
    if((index > 2) || (index < 0)){__debugbreak();}
    return ((float*)this)[index];
  }

  float Vector3::operator[](int index) const
  {
    if((index > 2) || (index < 0)){__debugbreak();}
    return ((float*)this)[index];
  }

}

RegisterType(WickedSick, Vector3)
RegisterMember(x);
RegisterMember(y);
RegisterMember(z);
}

