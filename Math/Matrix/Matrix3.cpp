#include "MathPrecompiled.h"
#include "Matrix3.h"
#include "Vector/Vector3.h"

#include "MathUtility.h"
namespace WickedSick
{
  Matrix3::Matrix3(void) 
                                : m00(0.0f), m01(0.0f), m02(0.0f),
                                  m10(0.0f), m11(0.0f), m12(0.0f),
                                  m20(0.0f), m21(0.0f), m22(0.0f)
  {
  }

  Matrix3::Matrix3(const Matrix3& rhs)
                                : m00(rhs.m00), m01(rhs.m01), m02(rhs.m02),
                                  m10(rhs.m10), m11(rhs.m11), m12(rhs.m12),
                                  m20(rhs.m20), m21(rhs.m21), m22(rhs.m22)
  {
  }

  Matrix3::Matrix3(float mm00, float mm01, float mm02,
                               float mm10, float mm11, float mm12,
                               float mm20, float mm21, float mm22)
                                : m00(mm00), m01(mm01), m02(mm02), 
                                  m10(mm10), m11(mm11), m12(mm12), 
                                  m20(mm20), m21(mm21), m22(mm22)
  {
  }

  //assignment
  Matrix3& Matrix3::operator=(const Matrix3& rhs)
  {
    if(this != &rhs)
    {
      m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02;
      m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12;
      m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22;
    }
    return *this;
  }

  void Matrix3::Transpose()
  {
    std::swap(m10, m01); std::swap(m20, m02);
                         std::swap(m21, m12);
  }

  void Matrix3::Inverse()
  {
    Identity();
  }

  //multiplication by vector / point
  Vector3 Matrix3::operator*(const Vector3& rhs) const
  {
    return Vector3(m00 * rhs.x + m01 * rhs.y + m02 * rhs.z,
                   m10 * rhs.x + m11 * rhs.y + m12 * rhs.z,
                   m20 * rhs.x + m21 * rhs.y + m22 * rhs.z);
  }

  //addition
  Matrix3 Matrix3::operator+(const Matrix3& rhs) const
  {
    return Matrix3(m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
                   m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
                   m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22);
  }

  //subtraction
  Matrix3 Matrix3::operator-(const Matrix3& rhs) const
  {
    return Matrix3(m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
                   m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
                   m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22);
  }

  //multiplication
  Matrix3 Matrix3::operator*(const Matrix3& rhs) const
  {
    return Matrix3(m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20, m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21, m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,
                   m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20, m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21, m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,
                   m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20, m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21, m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22);
  }

  //addition assignment
  Matrix3& Matrix3::operator+=(const Matrix3& rhs)
  {
    m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02;
    m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12;
    m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22;
    return *this;
  }

  //subtraction assignment
  Matrix3& Matrix3::operator-=(const Matrix3& rhs)
  {
    m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02;
    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12;
    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22;
    return *this;
  }

  //mulitplication assignment
  Matrix3& Matrix3::operator*=(const Matrix3& rhs)
  {
    (*this) = (*this) * rhs;
    return *this;
  }

  //scaling
  Matrix3 Matrix3::operator*(const float rhs) const
  {
    return Matrix3(m00 * rhs, m01 * rhs, m02 * rhs,
                   m10 * rhs, m11 * rhs, m12 * rhs,
                   m20 * rhs, m21 * rhs, m22 * rhs);
  }

  //dividing scalar
  Matrix3 Matrix3::operator/(const float rhs) const
  {
    return Matrix3(m00 / rhs, m01 / rhs, m02 / rhs,
                   m10 / rhs, m11 / rhs, m12 / rhs,
                   m20 / rhs, m21 / rhs, m22 / rhs);
  }

  //scale assignment
  Matrix3& Matrix3::operator*=(const float rhs)
  {
    m00 *= rhs; m01 *= rhs; m02 *= rhs; 
    m10 *= rhs; m11 *= rhs; m12 *= rhs; 
    m20 *= rhs; m21 *= rhs; m22 *= rhs; 
    return *this;
  }

  //dividing scale assignment
  Matrix3& Matrix3::operator/=(const float rhs)
  {
    m00 /= rhs; m01 /= rhs; m02 /= rhs; 
    m10 /= rhs; m11 /= rhs; m12 /= rhs; 
    m20 /= rhs; m21 /= rhs; m22 /= rhs; 
    return *this;
  }

  //equality
  bool Matrix3::operator==(const Matrix3& rhs) const
  {
    return (m00 - rhs.m00 < EPSILON && m00 - rhs.m00 > -EPSILON) && (m01 - rhs.m01 < EPSILON && m01 - rhs.m01 > -EPSILON) && (m02 - rhs.m02 < EPSILON && m02 - rhs.m02 > -EPSILON) &&
           (m10 - rhs.m10 < EPSILON && m10 - rhs.m10 > -EPSILON) && (m11 - rhs.m11 < EPSILON && m11 - rhs.m11 > -EPSILON) && (m12 - rhs.m12 < EPSILON && m12 - rhs.m12 > -EPSILON) &&
           (m20 - rhs.m20 < EPSILON && m20 - rhs.m20 > -EPSILON) && (m21 - rhs.m21 < EPSILON && m21 - rhs.m21 > -EPSILON) && (m22 - rhs.m22 < EPSILON && m22 - rhs.m22 > -EPSILON);
  }

  //inequality
  bool Matrix3::operator!=(const Matrix3& rhs) const
  {
    return !(rhs == *this);
  }

  void Matrix3::Zero(void)
  {
    m00 = m01 = m02 = 
    m10 = m11 = m12 = 
    m20 = m21 = m22 = 0.0f;
  }

  void Matrix3::Identity(void)
  {
          m01 = m02 = 
    m10 =       m12 =
    m20 = m21       = 0.0f;

    m00 = m11 = m22 = 1.0f;
  }

  void Matrix3::Print(void) const
  {
    //ConsolePrint("--------------------------");
    //ConsolePrint(std::to_string(m00) + " " + std::to_string(m01) + " " + std::to_string(m02));
    //ConsolePrint(std::to_string(m10) + " " + std::to_string(m11) + " " + std::to_string(m12));
    //ConsolePrint(std::to_string(m20) + " " + std::to_string(m21) + " " + std::to_string(m22));
    //ConsolePrint("--------------------------");
  }
}
