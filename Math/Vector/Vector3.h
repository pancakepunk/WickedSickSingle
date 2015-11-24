#pragma once

#include "meta/MetaInterface.h"


namespace WickedSick
{
  struct Vector2;
  struct Vector3i;
  struct Matrix3;
  struct Matrix4;

  struct Vector3
  {
    MetaDef;
    // Constructors
    Vector3(void);
    Vector3(const Vector2& rhs, float zz=0.0f);
    Vector3(const Vector3& rhs);
    Vector3(float xx, float yy, float zz);

    // Assignment operator, does not need to handle self assignment
    Vector3 & operator=(const Vector3& rhs);

    // Unary negation operator, negates all components and returns a copy
    Vector3 operator-(void) const;  

    // Math Operators
    Vector3 operator+(const Vector3& rhs) const;
    Vector3 operator-(const Vector3& rhs) const;
    Vector3 operator*(const Matrix4& rhs) const;
    Vector3 operator*(const Matrix3& rhs) const;
    Vector3 operator*(const float rhs) const;
    Vector3 operator/(const float rhs) const;
    Vector3 & operator+=(const Vector3& rhs);
    Vector3 & operator-=(const Vector3& rhs);
    Vector3 & operator*=(const float rhs);
    Vector3 & operator/=(const float rhs);

    // Comparison operators
    bool operator==(const Vector3& rhs) const;
    bool operator!=(const Vector3& rhs) const;
         
    // Linear Algebra
    float   Length(void) const;
    float   LengthSq(void) const;
    void    Negate(void);
    Vector3 GetNormalized(void) const; //returns a normalized version of the vector
    void    Normalize(void);
    float   Dot(const Vector3& rhs) const;
    Vector3 Cross(const Vector3& rhs) const;
    bool    IsZero(void);

    // Utility Functions
    float   AngleBetween(const Vector3 & other);
    void    RotateAround(const Vector3& axis, float amount);
    Vector3 GetRotatedAround(const Vector3& axis, float amount);

    // Settors
    void Zero(void);
    void Set(float xx=0.0f, float yy=0.0f, float zz=0.0f);

    // Helpers
    void    Print(void) const;
    Vector3 Min(const Vector3& rhs);
    Vector3 Max(const Vector3& rhs);

    Vector3i to_i();
    std::string to_string();

    float & operator[](int index);
    float   operator[](int index) const;

    float x;
    float y;
    float z;
  };

}


