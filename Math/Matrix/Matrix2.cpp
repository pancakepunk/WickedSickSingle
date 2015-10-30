#include "MathPrecompiled.h"
#include "Matrix2.h"

#include "Vector/Vector2.h"

#include "MathUtility.h"

namespace WickedSick
{
  Matrix2::Matrix2(void) : 
    m00(0.0f), m01(0.0f), m10(0.0f), m11(0.0f)
  {
  }

  Matrix2::Matrix2(const Matrix2& rhs) : 
    m00(rhs.m00), m01(rhs.m01), m10(rhs.m10), m11(rhs.m11)
  {
  }

  Matrix2::Matrix2(float mm00, float mm01, float mm10, float mm11) : 
    m00(mm00), m01(mm01), m10(mm10), m11(mm11)
  {
  }

  //assignment
  Matrix2& Matrix2::operator=(const Matrix2& rhs)
  {
    if(this != &rhs)
    {
      m00 = rhs.m00; m01 = rhs.m01;
      m10 = rhs.m10; m11 = rhs.m11; 
    }
    return *this;
  }

  void Matrix2::Transpose()
  {
    std::swap(m10, m01);    
  }

  void Matrix2::Inverse()
  {
    *this = Matrix2(m11, -m01, -m10, m00);
  }

  //multiplication by vector / point
  Vector2 Matrix2::operator*(const Vector2& rhs) const
  {
    return Vector2(m00 * rhs.x + m01 * rhs.y,
                   m10 * rhs.x + m11 * rhs.y);
  }

  //addition
  Matrix2 Matrix2::operator+(const Matrix2& rhs) const
  {
    return Matrix2(m00 + rhs.m00, m01 + rhs.m01,
                   m10 + rhs.m10, m11 + rhs.m11);
  }

  //subtraction
  Matrix2 Matrix2::operator-(const Matrix2& rhs) const
  {
    return Matrix2(m00 - rhs.m00, m01 - rhs.m01,
                   m10 - rhs.m10, m11 - rhs.m11);
  }

  //multiplication
  Matrix2 Matrix2::operator*(const Matrix2& rhs) const
  {
    return Matrix2(m00 * rhs.m00 + m01 * rhs.m10, m00 * rhs.m01 + m01 * rhs.m11,
                   m10 * rhs.m00 + m11 * rhs.m10, m10 * rhs.m01 + m11 * rhs.m11);
  }

  //addition assignment
  Matrix2& Matrix2::operator+=(const Matrix2& rhs)
  {
    m00 += rhs.m00; m01 += rhs.m01;
    m10 += rhs.m10; m11 += rhs.m11;
    return *this;
  }

  //subtraction assignment
  Matrix2& Matrix2::operator-=(const Matrix2& rhs)
  {
    m00 -= rhs.m00; m01 -= rhs.m01;
    m10 -= rhs.m10; m11 -= rhs.m11;
    return *this;
  }

  //mulitplication assignment
  Matrix2& Matrix2::operator*=(const Matrix2& rhs)
  {
    (*this) = (*this) * rhs;
    return *this;
  }

  //scaling
  Matrix2 Matrix2::operator*(const float rhs) const
  {
    return Matrix2(m00 * rhs, m01 * rhs,
                   m10 * rhs, m11 * rhs);
  }

  //dividing scalar
  Matrix2 Matrix2::operator/(const float rhs) const
  {
    return Matrix2(m00 / rhs, m01 / rhs,
                   m10 / rhs, m11 / rhs);
  }

  //scale assignment
  Matrix2& Matrix2::operator*=(const float rhs)
  {
    m00 *= rhs; m01 *= rhs; 
    m10 *= rhs; m11 *= rhs; 
    return *this;
  }

  //dividing scale assignment
  Matrix2& Matrix2::operator/=(const float rhs)
  {
    m00 /= rhs; m01 /= rhs;
    m10 /= rhs; m11 /= rhs;
    return *this;
  }

  //equality
  bool Matrix2::operator==(const Matrix2& rhs) const
  {
    return (m00 - rhs.m00 < EPSILON && m00 - rhs.m00 > -EPSILON) && (m01 - rhs.m01 < EPSILON && m01 - rhs.m01 > -EPSILON) &&
           (m10 - rhs.m10 < EPSILON && m10 - rhs.m10 > -EPSILON) && (m11 - rhs.m11 < EPSILON && m11 - rhs.m11 > -EPSILON);
  }

  //inequality
  bool Matrix2::operator!=(const Matrix2& rhs) const
  {
    return !(rhs == *this);
  }

  void Matrix2::Zero(void)
  {
    m00 = m01 = m10 = m11 = 0.0f;
  }
  void Matrix2::Identity(void)
  {
    m01 = m10 = 0.0f;
    m00 = m11 = 1.0f;
  }

  void Matrix2::Print(void) const
  {
    //ConsolePrint("--------------------------");
    //ConsolePrint(std::to_string(m00) + "  " + std::to_string(m01));
    //ConsolePrint(std::to_string(m10) + "  " + std::to_string(m11));
    //ConsolePrint("--------------------------");
  }
}