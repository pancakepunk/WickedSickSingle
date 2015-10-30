#pragma once

#include "meta/MetaInterface.h"
namespace WickedSick
{
  struct Vector2i;
  struct Vector3;
  struct Vector3i
  {
    MetaDef;
    // Constructors
    Vector3i(void);
    Vector3i(const Vector2i& rhs, int zz=0);
    Vector3i(const Vector3i& rhs);
    Vector3i(int xx, int yy, int zz);

    // Assignment operator, does not need to handle self assignment
    Vector3i & operator=(const Vector3i& rhs);

    // Unary negation operator, negates all components and returns a copy
    Vector3i  operator-(void) const;

    // Math Operators
    Vector3i operator+(const Vector3i& rhs) const;
    Vector3i operator-(const Vector3i& rhs) const;
    Vector3i operator*(const int rhs) const;
    Vector3i operator/(const int rhs) const;
    Vector3i & operator+=(const Vector3i& rhs);
    Vector3i & operator-=(const Vector3i& rhs);
    Vector3i & operator*=(const int rhs);
    Vector3i & operator/=(const int rhs);

    // Comparison operators
    bool operator==(const Vector3i& rhs) const;
    bool operator!=(const Vector3i& rhs) const;
         
    // Linear Algebra
    float     Length(void) const;
    int       LengthSq(void) const;
    void      Negate(void);
    Vector3   GetNormalized(void); //returns a normalized version of the vector
    int       Dot(const Vector3i& rhs) const;
    Vector3i  Cross(const Vector3i& rhs) const;

    // Settors
    void Zero(void);
    void Set(int xx=0, int yy=0, int zz=0);

    // Helpers
    void      Print(void) const;
    Vector3i  Min(const Vector3i& rhs);
    Vector3i  Max(const Vector3i& rhs);

    Vector3   to_f();
    std::string   to_string();

    int &  operator[](int index);
    int operator[](int index) const;

    int x;
    int y;
    int z;

  };
}
