#include "Precompiled.h"
///////////////////////////////////////////////////////////////////////////
//Author:      Mark
//Date:        4/4/2014
//Description: Vector 4 def
//All content (c) 2014 DigiPen (USA) Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////
#include "MathPrecompiled.h"
#include "Vector4.h"

#include "Vector/Vector4i.h"
#include "Vector/Vector3.h"
#include "Vector/Vector2.h"

#include "Matrix/Matrix4.h"
#include "MathUtility.h"

namespace WickedSick
{
  Vector4::Vector4(void) : 
    x(0.0f), y(0.0f), z(0.0f), w(1.0f)
  {
  }

  Vector4::Vector4(const Vector2& rhs, float zz, float ww) : 
    x(rhs.x), y(rhs.y), z(zz), w(ww)
  {
  }

  Vector4::Vector4( const Vector3& rhs, float ww ) : 
    x(rhs.x), y(rhs.y), z(rhs.z), w(ww)
  {
  }

  Vector4::Vector4(const Vector4& rhs) : 
    x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
  {
  }

  Vector4::Vector4(float xx, float yy, float zz, float ww) : 
    x(xx), y(yy), z(zz), w(ww)
  {
  }

  Vector4 & Vector4::operator=(const Vector4& rhs)
  {
    if(this != &rhs)
    {
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
      w = rhs.w;
    }
    return *this;
  }

  Vector4 Vector4::operator-(void) const
  {
    return Vector4(-x, -y, -z, -w);
  }

  Vector4 Vector4::operator+(const Vector4& rhs) const
  {
    return Vector4( x + rhs.x,
                    y + rhs.y, 
                    z + rhs.z,
                    w + rhs.w);
  }

  Vector4 Vector4::operator-(const Vector4& rhs) const
  {
    return Vector4( x - rhs.x,
                    y - rhs.y, 
                    z - rhs.z,
                    w - rhs.w);
  }

  Vector4 Vector4::operator*(const float rhs) const
  {
    return Vector4( x * rhs,
                    y * rhs, 
                    z * rhs,
                    w * rhs);
  }

  Vector4 Vector4::operator*(const Matrix4& rhs) const
  {
    return Vector4(x * rhs.m00 + y * rhs.m10 + z * rhs.m20 + w * rhs.m30,
                   x * rhs.m01 + y * rhs.m11 + z * rhs.m21 + w * rhs.m31,
                   x * rhs.m02 + y * rhs.m12 + z * rhs.m22 + w * rhs.m32,
                   x * rhs.m03 + y * rhs.m13 + z * rhs.m23 + w * rhs.m33);
  }

  Vector4 Vector4::operator*(const Vector4& rhs) const
  {
    return Vector4( x * rhs.x,
                    y * rhs.y, 
                    z * rhs.z, 
                    w * rhs.w);
  }

  Vector4 Vector4::operator/(const float rhs) const
  {
    return Vector4( x / rhs,
                    y / rhs, 
                    z / rhs, 
                    w / rhs);
  }


  Vector4 & Vector4::operator+=(const Vector4& rhs)
  {
    x += rhs.x;
    y += rhs.y; 
    z += rhs.z;
    w += rhs.w;
    return *this;
  }

  Vector4 & Vector4::operator-=(const Vector4& rhs)
  {
    x -= rhs.x;
    y -= rhs.y; 
    z -= rhs.z;
    w -= rhs.w;
    return *this;
  }

  Vector4 & Vector4::operator*=(const float rhs)
  {
    x *= rhs;
    y *= rhs; 
    z *= rhs;
    w *= rhs;
    return *this;
  }

  Vector4 & Vector4::operator/=(const float rhs)
  {
    if(rhs == 0)
    {
      Zero();
    }
    else
    {
      x /= rhs;
      y /= rhs; 
      z /= rhs;
      w /= rhs;
    }
    return *this;
  }

  bool Vector4::operator==(const Vector4& rhs) const
  {
    return  abs(x - rhs.x) <= EPSILON &&
            abs(y - rhs.y) <= EPSILON &&
            abs(z - rhs.z) <= EPSILON &&
            abs(w - rhs.w) <= EPSILON;
  }

  bool Vector4::operator!=(const Vector4& rhs) const
  {
    return  abs(x - rhs.x) > EPSILON ||
            abs(y - rhs.y) > EPSILON ||
            abs(z - rhs.z) > EPSILON ||
            abs(w - rhs.w) > EPSILON;
  }

  float Vector4::Length(void) const
  {
    return sqrt(x * x + y * y + z * z);
  }

  float Vector4::LengthSq(void) const
  {
    return x * x + y * y + z * z;
  }

  void Vector4::Negate(void)
  {
    x = -x; y = -y; z = -z; w = -w;
  }

  void Vector4::Maximize(void)
  {
    x = y = z = FLT_MAX;
  }

  void Vector4::Minimize(void)
  {
    x = y = z = FLT_MIN;
  }

  Vector4 Vector4::GetNormalized(void)
  {
    float len = Length();
    if(len == 0)
    {
      //Warn("error: normalizing a zero vector");
      return Vector4();
    }
    return Vector4(x/len, y/len, z/len, w/len);
  }

  void Vector4::Normalize(void)
  {
    float len = Length();
    if(len == 0)
    {
      //Warn("error: normalizing a zero vector");
      return;
    }
    x /=  len; 
    y /=  len; 
    z /=  len; 
    w /=  len;
  }

  float Vector4::Dot(const Vector4& rhs) const
  {
    return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);// + (w * rhs.w);
  }

  Vector4 Vector4::Cross(const Vector4& rhs) const
  {
    return Vector4( y * rhs.z - z * rhs.y,
                    z * rhs.x - x * rhs.z,
                    x * rhs.y - y * rhs.x, 
                    1.0f);
  }

  bool Vector4::IsZero(void)
  {
    if(x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) return true;
    else                                                 return false;
  }

  void Vector4::Zero(void)
  {
    x = y = z = 0.0f;
  }

  void Vector4::Set(float xx, float yy, float zz, float ww)
  {
    x = xx; y = yy; z = zz; w = ww;
  }

  void Vector4::Print(void) const
  {
    ConsolePrint(std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w));
  }

  Vector4 Vector4::Min(const Vector4& rhs)
  {
    return Vector4(std::min(x, rhs.x), std::min(y, rhs.y), std::min(z, rhs.z), std::min(w, rhs.w));
  }

  Vector4 Vector4::Max(const Vector4& rhs)
  {
    return Vector4(std::max(x, rhs.x), std::max(y, rhs.y), std::max(z, rhs.z), std::max(w, rhs.w));
  }

  Vector4i Vector4::to_i()
  {
    return Vector4i(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z), static_cast<int>(w));
  }

  Vector3 Vector4::xyz()
  {
    return Vector3(x, y, z);
  }

  Vector2 Vector4::xy()
  {
    return Vector2(x, y);
  }

  std::string Vector4::to_string()
  {
    return std::to_string(x) + std::to_string(y) + std::to_string(z) + std::to_string(w);
  }

  float & Vector4::operator [](int index)
  {
    if((index > 3) || (index < 0)){__debugbreak();}
    return ((float*)this)[index];
  }

  float Vector4::operator[](int index) const
  {
    if((index > 3) || (index < 0)){__debugbreak();}
    return ((float*)this)[index];
  }
  Vector4 operator*(float f, Vector4 const & v)
  {
    return v*f;
  }
}

RegisterType(WickedSick, Vector4)
RegisterMember(x);
RegisterMember(y);
RegisterMember(z);
RegisterMember(w);
}
