#include "Precompiled.h"
///////////////////////////////////////////////////////////////////////////
//Author:      Mark
//Date:        4/4/2014
//Description: vector 2 implementation
//All content (c) 2014 DigiPen (USA) Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////
#include "MathPrecompiled.h"
#include "Vector2.h"

#include "Vector/Vector2i.h"

#include "MathUtility.h"

namespace WickedSick
{
  Vector2::Vector2(void) : x(0.0f), y(0.0f)
  {
  }

  Vector2::Vector2(const Vector2& rhs) : x(rhs.x), y(rhs.y)
  {
  }
 

  Vector2::Vector2(float xx, float yy) : x(xx), y(yy)
  {
  }

  Vector2 & Vector2::operator=(const Vector2& rhs)
  {
    if(this != &rhs)
    {
      x = rhs.x;
      y = rhs.y;
    }
    return *this;
  }

  Vector2 Vector2::operator-(void) const
  {
    return Vector2(-x, -y);
  }

  Vector2 Vector2::operator+(const Vector2& rhs) const
  {
    return Vector2(x + rhs.x, y + rhs.y);
  }

  Vector2 Vector2::operator-(const Vector2& rhs) const
  {
    return Vector2(x - rhs.x, y - rhs.y);
  }

  Vector2 Vector2::operator*(const float rhs) const
  {
    return Vector2(x * rhs, y * rhs);
  }

  Vector2 Vector2::operator/(const float rhs) const
  {
    return Vector2(x / rhs, y / rhs);
  }


  Vector2 & Vector2::operator+=(const Vector2& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Vector2 & Vector2::operator-=(const Vector2& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  Vector2 & Vector2::operator*=(const float rhs)
  {
    x *= rhs;
    y *= rhs;
    return *this;
  }

  Vector2 & Vector2::operator/=(const float rhs)
  {
    x /= rhs;
    y /= rhs;
    return *this;
  }

  bool Vector2::operator==(const Vector2& rhs) const
  {
    return abs(x - rhs.x) <= EPSILON &&
           abs(y - rhs.y) <= EPSILON;
  }

  bool Vector2::operator!=(const Vector2& rhs) const
  {
    return abs(x - rhs.x) > EPSILON ||
           abs(y - rhs.y) > EPSILON;
  }


  Vector2 Vector2::GetNormalized(void)
  {
    float len = Length();
    if(len == 0.0f)
    {
      return *this;
    }    
    return Vector2(x / len, y / len);
  }

  void Vector2::Normalize(void)
  {
    float len = Length();
    if(len == 0.0f)
    {
      return;
    }   
    x /= len;
    y /= len;
  }
  
  float Vector2::Dot(const Vector2& rhs) const
  {
    return (x * rhs.x) + (y * rhs.y);
  }

  float Vector2::Cross(const Vector2& rhs) const
  {
    return (x * rhs.y) - (y * rhs.x);
  }

  bool Vector2::IsZero(void)
  {
    if(x == 0.0f && y == 0.0f) return true;
    else                       return false;
  }
   
  void Vector2::Zero(void)
  {
    x = y = 0.0f;
  }

  void Vector2::Negate(void)
  {
    x = -x; y = -y;
  }

  float Vector2::Length(void) const
  {
    return sqrt(LengthSq());
  }
  
  float Vector2::LengthSq(void) const
  {
    return (x * x) + (y * y);
  }
  
  void Vector2::Print(void) const
  {
    ConsolePrint(std::to_string(x) + ", " + std::to_string(y) + "\n");
  }

  Vector2 Vector2::Min(const Vector2& rhs)
  {
    return Vector2(std::min(x, rhs.x), std::min(y, rhs.y));
  }

  Vector2 Vector2::Max(const Vector2& rhs)
  {
    return Vector2(std::max(x, rhs.x), std::max(y, rhs.y));
  }


  Vector2i Vector2::to_i()
  {
    return Vector2i(static_cast<int>(x), static_cast<int>(y));
  }

  std::string Vector2::to_string()
  {
    return std::to_string(x) + std::to_string(y);
  }

  float & Vector2::operator [](int index)
  {
    if((index > 1) || (index < 0)){__debugbreak();}
    return ((float*)this)[index];
  }

  float Vector2::operator[](int index) const
  {
    if((index > 1) || (index < 0)){__debugbreak();}
    return ((float*)this)[index];
  }

  Vector2 operator*(float f, Vector2 const & v)
  {
    return v * f;
  }

}

RegisterType(WickedSick, Vector2)
RegisterMember(x);
RegisterMember(y);
}

