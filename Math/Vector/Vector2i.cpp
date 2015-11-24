#include "Precompiled.h"
///////////////////////////////////////////////////////////////////////////
//Author:      Zhuhuii
//Date:        09/2/2014
//Description: 
//All content (c) 2014 DigiPen (USA) Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////
#include "MathPrecompiled.h"
#include "Vector2i.h"

#include "Vector/Vector2.h"


#include "MathUtility.h"

namespace WickedSick
{
  Vector2i::Vector2i(void) : x(0), y(0)
  {
  }

  Vector2i::Vector2i(const Vector2i& rhs) : x(rhs.x), y(rhs.y)
  {
  }

  Vector2i::Vector2i(int xx, int yy) : x(xx), y(yy)
  {
  }

  Vector2i & Vector2i::operator=(const Vector2i& rhs)
  {
    if(this != &rhs)
    {
      x = rhs.x;
      y = rhs.y;
    }
    return *this;
  }

  Vector2i Vector2i::operator-(void) const
  {
    return Vector2i(-x, -y);
  }

  Vector2i Vector2i::operator+(const Vector2i& rhs) const
  {
    return Vector2i(x + rhs.x, y + rhs.y);
  }

  Vector2i Vector2i::operator-(const Vector2i& rhs) const
  {
    return Vector2i(x - rhs.x, y - rhs.y);
  }

  Vector2i Vector2i::operator*(const int rhs) const
  {
    return Vector2i(x * rhs, y * rhs);
  }

  Vector2i Vector2i::operator/(const int rhs) const
  {
    return Vector2i(x / rhs, y / rhs);
  }


  Vector2i & Vector2i::operator+=(const Vector2i& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Vector2i & Vector2i::operator-=(const Vector2i& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  Vector2i & Vector2i::operator*=(const int rhs)
  {
    x *= rhs;
    y *= rhs;
    return *this;
  }

  Vector2i & Vector2i::operator/=(const int rhs)
  {
    x /= rhs;
    y /= rhs;
    return *this;
  }

  bool Vector2i::operator==(const Vector2i& rhs) const
  {
    return abs(x - rhs.x) <= EPSILON &&
           abs(y - rhs.y) <= EPSILON;
  }

  bool Vector2i::operator!=(const Vector2i& rhs) const
  {
    return abs(x - rhs.x) > EPSILON ||
           abs(y - rhs.y) > EPSILON;
  }

  Vector2i operator*(int f, Vector2i const & v)
  {
    return Vector2i(v.x * f, v.y * f);
  }


  Vector2 Vector2i::GetNormalized(void) const
  {
    Vector2 floats = to_f();
    floats.Normalize();
    return floats;
  }
  
  int Vector2i::Dot(const Vector2i& rhs) const
  {
    return (x * rhs.x) + (y * rhs.y);
  }

  int Vector2i::Cross(const Vector2i& rhs) const
  {
    return (x * rhs.y) - (y * rhs.x);
  }

  void Vector2i::Zero(void)
  {
    x = y = 0;
  }

  void Vector2i::Set(int xx, int yy)
  {
    x = xx; y = yy;
  }

  void Vector2i::Negate(void)
  {
    x = -x; y = -y;
  }

  float Vector2i::Length(void) const
  {
    return sqrt(static_cast<float>(LengthSq()));
  }
  
  int Vector2i::LengthSq(void) const
  {
    return (x * x) + (y * y);
  }
  
  void Vector2i::Print(void) const
  {
    ConsolePrint(std::to_string(x) + ", " + std::to_string(y));
  }

  Vector2i Vector2i::Min(const Vector2i& rhs)
  {
    return Vector2i(std::min(x, rhs.x), std::min(y, rhs.y));
  }

  Vector2i Vector2i::Max(const Vector2i& rhs)
  {
    return Vector2i(std::max(x, rhs.x), std::max(y, rhs.y));
  }

  Vector2 Vector2i::to_f() const
  {
    return Vector2(static_cast<float>(x), static_cast<float>(y));
  }

  std::string Vector2i::to_string() const
  {
    return std::to_string(x) + std::to_string(y);
  }

  int& Vector2i::operator[](int index)
  {
    if((index > 1) || (index < 0)){__debugbreak();}
    return ((int*)this)[index];
  }

  int Vector2i::operator[](int index) const
  {
    if((index > 1) || (index < 0)){__debugbreak();}
    return ((int*)this)[index];
  }
}
