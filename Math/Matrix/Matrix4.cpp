///////////////////////////////////////////////////////////////////////////
//Author:      Mark
//Date:        4/4/2014
//Description: matrix 4 impementaiton
//All content (c) 2014 DigiPen (USA) Corporation, all rights reserved.
////////////////////////////////// /////////////////////////////////////////
#include "MathPrecompiled.h"
#include "Matrix4.h"

#include "Math/Vector/Vector4.h"
#include "Math/Vector/Vector3.h"

#include "Math/MathUtility.h"


#include <utility>
#include <cmath>

namespace WickedSick
{
  Matrix4::Matrix4(void) : m00(0.0f), m01(0.0f), m02(0.0f), m03(0.0f),
                                       m10(0.0f), m11(0.0f), m12(0.0f), m13(0.0f),
                                       m20(0.0f), m21(0.0f), m22(0.0f), m23(0.0f),
                                       m30(0.0f), m31(0.0f), m32(0.0f), m33(0.0f)
  {
  }

  Matrix4::Matrix4(const Matrix4& rhs) : m00(rhs.m00), m01(rhs.m01), m02(rhs.m02), m03(rhs.m03),
                                                     m10(rhs.m10), m11(rhs.m11), m12(rhs.m12), m13(rhs.m13),
                                                     m20(rhs.m20), m21(rhs.m21), m22(rhs.m22), m23(rhs.m23),
                                                     m30(rhs.m30), m31(rhs.m31), m32(rhs.m32), m33(rhs.m33)
  {
  }

  Matrix4::Matrix4(const float* f) 
  {
    *this = *(const Matrix4*)f;
  }

  Matrix4::Matrix4(float mm00, float mm01, float mm02, float mm03,
                               float mm10, float mm11, float mm12, float mm13,
                               float mm20, float mm21, float mm22, float mm23,
                               float mm30, float mm31, float mm32, float mm33) : m00(mm00), m01(mm01), m02(mm02), m03(mm03),
                                                                                 m10(mm10), m11(mm11), m12(mm12), m13(mm13),
                                                                                 m20(mm20), m21(mm21), m22(mm22), m23(mm23),
                                                                                 m30(mm30), m31(mm31), m32(mm32), m33(mm33)
  {
  }

  //assignment
  Matrix4& Matrix4::operator=(const Matrix4& rhs)
  {
    if(this != &rhs)
    {
      m00 = rhs.m00; m01 = rhs.m01; m02 = rhs.m02; m03 = rhs.m03;
      m10 = rhs.m10; m11 = rhs.m11; m12 = rhs.m12; m13 = rhs.m13;
      m20 = rhs.m20; m21 = rhs.m21; m22 = rhs.m22; m23 = rhs.m23;
      m30 = rhs.m30; m31 = rhs.m31; m32 = rhs.m32; m33 = rhs.m33;
    }
    return *this;
  }

  Matrix4 Matrix4::GetTranspose() const
  {
    Matrix4 toReturn(*this);
    toReturn.Transpose();
    return toReturn;
  }

  void Matrix4::Transpose()
  {
    std::swap(m10, m01); std::swap(m20, m02); std::swap(m30, m03);
    std::swap(m21, m12); std::swap(m31, m13);
    std::swap(m32, m23);
  }

  void Matrix4::Inverse()
  {
    float const *original = v;		// cast the matrix as a flat array
    float temp[12];				// used for intermediate calculations
    float src[16];				// transpose of original matrix inverse

    // transpose the original matrix
    for( int i =  0; i < 4; i++ )
    {
      src[i]      = original[i * 4];
      src[i + 4]  = original[i * 4 + 1];
      src[i + 8]  = original[i * 4 + 2];
      src[i + 12] = original[i * 4 + 3];
    }

    // calculate pairs for the first 8 elements (cofactors)
    temp[0]  = src[10] * src[15];
    temp[1]  = src[11] * src[14];
    temp[2]  = src[9]  * src[15];
    temp[3]  = src[11] * src[13];
    temp[4]  = src[9]  * src[14];
    temp[5]  = src[10] * src[13];
    temp[6]  = src[8]  * src[15];
    temp[7]  = src[11] * src[12];
    temp[8]  = src[8]  * src[14];
    temp[9]  = src[10] * src[12];
    temp[10] = src[8]  * src[13];
    temp[11] = src[9]  * src[12];

    // calculate first 8 elements (cofactors)
    v[0]  = temp[0] * src[5] + temp[3] * src[6] + temp[4]  * src[7];
    v[0] -= temp[1] * src[5] + temp[2] * src[6] + temp[5]  * src[7];
    v[1]  = temp[1] * src[4] + temp[6] * src[6] + temp[9]  * src[7];
    v[1] -= temp[0] * src[4] + temp[7] * src[6] + temp[8]  * src[7];
    v[2]  = temp[2] * src[4] + temp[7] * src[5] + temp[10] * src[7];
    v[2] -= temp[3] * src[4] + temp[6] * src[5] + temp[11] * src[7];
    v[3]  = temp[5] * src[4] + temp[8] * src[5] + temp[11] * src[6];
    v[3] -= temp[4] * src[4] + temp[9] * src[5] + temp[10] * src[6];
    v[4]  = temp[1] * src[1] + temp[2] * src[2] + temp[5]  * src[3];
    v[4] -= temp[0] * src[1] + temp[3] * src[2] + temp[4]  * src[3];
    v[5]  = temp[0] * src[0] + temp[7] * src[2] + temp[8]  * src[3];
    v[5] -= temp[1] * src[0] + temp[6] * src[2] + temp[9]  * src[3];
    v[6]  = temp[3] * src[0] + temp[6] * src[1] + temp[11] * src[3];
    v[6] -= temp[2] * src[0] + temp[7] * src[1] + temp[10] * src[3];
    v[7]  = temp[4] * src[0] + temp[9] * src[1] + temp[10] * src[2];
    v[7] -= temp[5] * src[0] + temp[8] * src[1] + temp[11] * src[2];

    // calculate pairs for second 8 elements (cofactors)
    temp[0] = src[2] * src[7];
    temp[1] = src[3] * src[6];
    temp[2] = src[1] * src[7];
    temp[3] = src[3] * src[5];
    temp[4] = src[1] * src[6];
    temp[5] = src[2] * src[5];
    temp[6] = src[0] * src[7];
    temp[7] = src[3] * src[4];
    temp[8] = src[0] * src[6];
    temp[9] = src[2] * src[4];
    temp[10] = src[0] * src[5];
    temp[11] = src[1] * src[4];

    // calculate second 8 elements (cofactors)
    v[8]   = temp[0]  * src[13] + temp[3]  * src[14] + temp[4]  * src[15];
    v[8]  -= temp[1]  * src[13] + temp[2]  * src[14] + temp[5]  * src[15];
    v[9]   = temp[1]  * src[12] + temp[6]  * src[14] + temp[9]  * src[15];
    v[9]  -= temp[0]  * src[12] + temp[7]  * src[14] + temp[8]  * src[15];
    v[10]  = temp[2]  * src[12] + temp[7]  * src[13] + temp[10] * src[15];
    v[10] -= temp[3]  * src[12] + temp[6]  * src[13] + temp[11] * src[15];
    v[11]  = temp[5]  * src[12] + temp[8]  * src[13] + temp[11] * src[14];
    v[11] -= temp[4]  * src[12] + temp[9]  * src[13] + temp[10] * src[14];
    v[12]  = temp[2]  * src[10] + temp[5]  * src[11] + temp[1]  * src[9];
    v[12] -= temp[4]  * src[11] + temp[0]  * src[9]  + temp[3]  * src[10];
    v[13]  = temp[8]  * src[11] + temp[0]  * src[8]  + temp[7]  * src[10];
    v[13] -= temp[6]  * src[10] + temp[9]  * src[11] + temp[1]  * src[8];
    v[14]  = temp[6]  * src[9]  + temp[11] * src[11] + temp[3]  * src[8];
    v[14] -= temp[10] * src[11] + temp[2]  * src[8]  + temp[7]  * src[9];
    v[15]  = temp[10] * src[10] + temp[4]  * src[8]  + temp[9]  * src[9];
    v[15] -= temp[8]  * src[9]  + temp[11] * src[10] + temp[5]  * src[8];

    // calculate determinant
    float det = src[0] * v[0] + src[1] * v[1] + src[2] * v[2] + src[3] * v[3];

    if(det == 0) 
    {
      return;
    }

    det =  1.0f / det;
    *this *= det;
  }

  //multiplication by vector / point
  Vector4 Matrix4::operator*(const Vector4& rhs) const
  {
    return Vector4(m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03 *rhs.w,
                   m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13 *rhs.w,
                   m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23 *rhs.w,
                   m30 * rhs.x + m31 * rhs.y + m32 * rhs.z + m33 *rhs.w);
  }

  //addition
  Matrix4 Matrix4::operator+(const Matrix4& rhs) const
  {
    return Matrix4(m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
                   m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
                   m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
                   m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33);
  }

  //subtraction
  Matrix4 Matrix4::operator-(const Matrix4& rhs) const
  {
    return Matrix4(m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
                   m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
                   m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
                   m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33);
  }

  //multiplication
  Matrix4 Matrix4::operator*(const Matrix4& rhs) const
  {
    return Matrix4(m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30, m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31, m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32, m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33,
                   m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30, m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31, m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32, m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33,
                   m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30, m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31, m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32, m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33,
                   m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30, m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31, m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32, m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33);
  }

  //addition assignment
  Matrix4& Matrix4::operator+=(const Matrix4& rhs)
  {
    m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03;
    m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
    m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
    m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
    return *this;
  }

  //subtraction assignment
  Matrix4& Matrix4::operator-=(const Matrix4& rhs)
  {
    m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03;
    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
    m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
    return *this;
  }

  //mulitplication assignment
  Matrix4& Matrix4::operator*=(const Matrix4& rhs)
  {
    (*this) = (*this) * rhs;
    return *this;
  }

  //scaling
  Matrix4 Matrix4::operator*(const float rhs) const
  {
    return Matrix4(m00 * rhs, m01 * rhs, m02 * rhs, m03 * rhs,
                   m10 * rhs, m11 * rhs, m12 * rhs, m13 * rhs,
                   m20 * rhs, m21 * rhs, m22 * rhs, m23 * rhs,
                   m30 * rhs, m31 * rhs, m32 * rhs, m33 * rhs);
  }

  //dividing scalar
  Matrix4 Matrix4::operator/(const float rhs) const
  {
    return Matrix4(m00 / rhs, m01 / rhs, m02 / rhs, m03 / rhs,
                   m10 / rhs, m11 / rhs, m12 / rhs, m13 / rhs,
                   m20 / rhs, m21 / rhs, m22 / rhs, m23 / rhs,
                   m30 / rhs, m31 / rhs, m32 / rhs, m33 / rhs);
  }

  //scale assignment
  Matrix4& Matrix4::operator*=(const float rhs)
  {
    m00 *= rhs; m01 *= rhs; m02 *= rhs; m03 *= rhs;
    m10 *= rhs; m11 *= rhs; m12 *= rhs; m13 *= rhs;
    m20 *= rhs; m21 *= rhs; m22 *= rhs; m23 *= rhs;
    m30 *= rhs; m31 *= rhs; m32 *= rhs; m33 *= rhs;
    return *this;
  }

  //dividing scale assignment
  Matrix4& Matrix4::operator/=(const float rhs)
  {
    m00 /= rhs; m01 /= rhs; m02 /= rhs; m03 /= rhs;
    m10 /= rhs; m11 /= rhs; m12 /= rhs; m13 /= rhs;
    m20 /= rhs; m21 /= rhs; m22 /= rhs; m23 /= rhs;
    m30 /= rhs; m31 /= rhs; m32 /= rhs; m33 /= rhs;
    return *this;
  }

  //equality
  bool Matrix4::operator==(const Matrix4& rhs) const
  {
    return (m00 - rhs.m00 < EPSILON && m00 - rhs.m00 > -EPSILON) && (m01 - rhs.m01 < EPSILON && m01 - rhs.m01 > -EPSILON) && (m02 - rhs.m02 < EPSILON && m02 - rhs.m02 > -EPSILON) && (m03 - rhs.m03 < EPSILON && m03 - rhs.m03 > -EPSILON) &&
           (m10 - rhs.m10 < EPSILON && m10 - rhs.m10 > -EPSILON) && (m11 - rhs.m11 < EPSILON && m11 - rhs.m11 > -EPSILON) && (m12 - rhs.m12 < EPSILON && m12 - rhs.m12 > -EPSILON) && (m13 - rhs.m13 < EPSILON && m13 - rhs.m13 > -EPSILON) &&
           (m20 - rhs.m20 < EPSILON && m20 - rhs.m20 > -EPSILON) && (m21 - rhs.m21 < EPSILON && m21 - rhs.m21 > -EPSILON) && (m22 - rhs.m22 < EPSILON && m22 - rhs.m22 > -EPSILON) && (m23 - rhs.m23 < EPSILON && m23 - rhs.m23 > -EPSILON) &&
           (m30 - rhs.m30 < EPSILON && m30 - rhs.m30 > -EPSILON) && (m31 - rhs.m31 < EPSILON && m31 - rhs.m31 > -EPSILON) && (m32 - rhs.m32 < EPSILON && m32 - rhs.m32 > -EPSILON) && (m33 - rhs.m33 < EPSILON && m33 - rhs.m33 > -EPSILON);
  }

  //inequality
  bool Matrix4::operator!=(const Matrix4& rhs) const
  {
    return (m00 - rhs.m00 >= EPSILON || m00 - rhs.m00 <= -EPSILON) || (m01 - rhs.m01 >= EPSILON || m01 - rhs.m01 <= -EPSILON) || (m02 - rhs.m02 >= EPSILON || m02 - rhs.m02 <= -EPSILON) || (m03 - rhs.m03 >= EPSILON || m03 - rhs.m03 <= -EPSILON) ||
           (m10 - rhs.m10 >= EPSILON || m10 - rhs.m10 <= -EPSILON) || (m11 - rhs.m11 >= EPSILON || m11 - rhs.m11 <= -EPSILON) || (m12 - rhs.m12 >= EPSILON || m12 - rhs.m12 <= -EPSILON) || (m13 - rhs.m13 >= EPSILON || m13 - rhs.m13 <= -EPSILON) ||
           (m20 - rhs.m20 >= EPSILON || m20 - rhs.m20 <= -EPSILON) || (m21 - rhs.m21 >= EPSILON || m21 - rhs.m21 <= -EPSILON) || (m22 - rhs.m22 >= EPSILON || m22 - rhs.m22 <= -EPSILON) || (m23 - rhs.m23 >= EPSILON || m23 - rhs.m23 <= -EPSILON) ||
           (m30 - rhs.m30 >= EPSILON || m30 - rhs.m30 <= -EPSILON) || (m31 - rhs.m31 >= EPSILON || m31 - rhs.m31 <= -EPSILON) || (m32 - rhs.m32 >= EPSILON || m32 - rhs.m32 <= -EPSILON) || (m33 - rhs.m33 >= EPSILON || m33 - rhs.m33 <= -EPSILON);
  }

  Matrix4& Matrix4::Zero(void)
  {
    m00 = m01 = m02 = m03 = 
    m10 = m11 = m12 = m13 =
    m20 = m21 = m22 = m23 =
    m30 = m31 = m32 = m33 = 0.0f;
    return *this;
  }

  Matrix4& Matrix4::Identity(void)
  {
    m01 = m02 = m03 = 
    m10 =       m12 = m13 =
    m20 = m21 =       m23 =
    m30 = m31 = m32 =       0.0f;

    m00 = m11 = m22 = m33 = 1.0f;
    return *this;
  }


  Matrix4& Matrix4::ScaleMat(const Vector3& scale)
  {
    *this = GetScaled(scale);
    return *this;
  }

  Matrix4 Matrix4::GetScaled(const Vector3& scale) const
  {
    Matrix4 scaleMat;
    scaleMat.Identity();
    scaleMat.m[0][0] = scale.x;
    scaleMat.m[1][1] = scale.y;
    scaleMat.m[2][2] = scale.z;

    return (scaleMat * *this);
  }


  Matrix4& Matrix4::RotateX(float x)
  {
    Matrix4 rotation;
    rotation.Identity();
    rotation.m11 = cos(x); rotation.m12 = -sin(x);
    rotation.m21 = sin(x); rotation.m22 = cos(x);

    *this = rotation * *this;
    //*this *= rotation;
    return *this;
  }

  Matrix4& Matrix4::RotateY(float y)
  {
    Matrix4 rotation;
    rotation.Identity();
    rotation.m00 = cos(y);  rotation.m02 = sin(y);
    rotation.m20 = -sin(y); rotation.m22 = cos(y);

    *this = rotation * *this;
    return *this;
  }

  Matrix4& Matrix4::RotateZ(float z)
  {
    Matrix4 rotation;
    rotation.Identity();
    rotation.m00 = cos(z); rotation.m01 =  -sin(z);
    rotation.m10 = sin(z); rotation.m11 =   cos(z);

    *this = rotation * *this;
    return *this;
  }

  Matrix4 Matrix4::GetRotatedX(float x) const
  {
    Matrix4 rotation;
    rotation.Identity();
    rotation.m11 = cos(x); rotation.m12 = -sin(x);
    rotation.m21 = sin(x); rotation.m22 = cos(x);

    return rotation * *this;
  }

  Matrix4 Matrix4::GetRotatedY(float y) const
  {
    Matrix4 rotation;
    rotation.Identity();
    rotation.m00 = cos(y);  rotation.m02 = sin(y);
    rotation.m20 = -sin(y); rotation.m22 = cos(y);

    return rotation * *this;
  }

  Matrix4 Matrix4::GetRotatedZ(float z) const
  {
    Matrix4 rotation;
    rotation.Identity();
    rotation.m00 = cos(z); rotation.m01 =  -sin(z);
    rotation.m10 = sin(z); rotation.m11 =   cos(z);

    return rotation * *this;
  }

  Matrix4& Matrix4::RotateXYZ(const Vector3& rot)
  {
    //RotateX(rot.x);
    RotateY(rot.y);
    //RotateZ(rot.z);
    return *this;
  }

  Matrix4 Matrix4::GetRotatedXYZ(const Vector3& rot) const
  {

    Matrix4 rotation;
    rotation.Identity();
    rotation.RotateX(rot.x);
    rotation.RotateY(rot.y);
    rotation.RotateZ(rot.z);
    return rotation * *this;
  }

  Matrix4& Matrix4::Translate(const Vector3& dist)
  {
    *this = GetTranslated(dist);
    return *this;
  }

  Matrix4 Matrix4::GetTranslated(const Vector3& dist) const
  {
    Matrix4 translateMat;
    translateMat.Identity();
    translateMat.m[0][3] += dist.x;
    translateMat.m[1][3] += dist.y;
    translateMat.m[2][3] += dist.z;

    return (translateMat * *this);

    
  }

  void Matrix4::Print(void) const
  {
    //ConsolePrint("--------------------------");
    //ConsolePrint(std::to_string(m00) + " " + std::to_string(m01) + " " + std::to_string(m02) + " " + std::to_string(m03));
    //ConsolePrint(std::to_string(m10) + " " + std::to_string(m11) + " " + std::to_string(m12) + " " + std::to_string(m13));
    //ConsolePrint(std::to_string(m20) + " " + std::to_string(m21) + " " + std::to_string(m22) + " " + std::to_string(m23));
    //ConsolePrint(std::to_string(m30) + " " + std::to_string(m31) + " " + std::to_string(m32) + " " + std::to_string(m33));
    //ConsolePrint("--------------------------");
  }


  Matrix4& Matrix4::DoPerspective(float fovW, float znear, float zfar, float aspectRatio)
  {

    Identity();
    //return *this;
    //fovW = 90;

    float yScale = 1.0f / tan(fovW / 2.0f);
    float xScale = yScale / aspectRatio;

    *this = Matrix4(xScale,   0,      0,                                0,
                    0,        yScale, 0,                                0,
                    0,        0,      zfar / (zfar - znear),            1,
                    0,        0,      (-znear * zfar) / (zfar - znear), 0);
    Transpose();
    //Zero();
    //fovW *= 0.5f;
    //float yScale = 1.0f/tan(fovW);
    //m[0][0] = yScale / aspectRatio;
    //                                m[1][1] = yScale;
    //                                                  m[2][2] = zfar / (znear - zfar);            m[2][3] = (znear * zfar) / (znear - zfar);
    //                                                  m[3][2] = 1.0f;

    //m[0][0] = yScale / aspectRatio;
    //                                m[1][1] = yScale;
    //                                                  m[2][2] = -(zfar + znear) / (zfar - znear);            m[2][3] = (-2 * znear * zfar) / (zfar - znear);
    //                                                  m[3][2] = -1.0f;
    return *this;
  }

  Matrix4& Matrix4::DoOrthographic(int width, int height, float znear, float zfar)
  {
    Zero();
    m[0][0] = 2.0f/(float)width;  
                          m[1][1] = 2.0f/(float)height; 
                                                  m[2][2] = 1.0f / (zfar - znear);
                                                  m[3][2] = znear / (znear - zfar); m[3][3] = 1.0f;
    return *this;
    
  }




}