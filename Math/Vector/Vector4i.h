#pragma once

#include "meta/MetaInterface.h"
namespace WickedSick
{
  struct Vector2i;
  struct Vector3i;
  struct Vector4;

  struct Vector4i
  {
    MetaDef;
    // Constructors
    Vector4i(void);
    Vector4i(const Vector2i& rhs, int zz=0, int ww=1);
    Vector4i(const Vector3i& rhs, int ww=1);
    Vector4i(const Vector4i& rhs);
    Vector4i(int xx, int yy, int zz, int ww);

    // Assignment operator, does not need to handle self assignment
    Vector4i &  operator=(const Vector4i& rhs);

    // Unary negation operator, negates all components and returns a copy
    Vector4i operator-(void) const;

    // Math Operators
    Vector4i operator+(const Vector4i& rhs) const;
    Vector4i operator-(const Vector4i& rhs) const;
    Vector4i operator*(const Vector4i& rhs) const;
    Vector4i operator*(const int rhs) const;
    Vector4i operator/(const int rhs) const;
    Vector4i & operator+=(const Vector4i& rhs);
    Vector4i & operator-=(const Vector4i& rhs);
    Vector4i & operator*=(const int rhs);
    Vector4i & operator/=(const int rhs);

    // Comparison operators
    bool operator==(const Vector4i& rhs) const;
    bool operator!=(const Vector4i& rhs) const;
         
    // Linear Algebra
    float     Length(void) const;
    int       LengthSq(void) const;
    void      Negate(void);
    Vector4   GetNormalized(void); //returns a normalized version of the vector
    int       Dot(const Vector4i& rhs) const;
    Vector4i  Cross(const Vector4i& rhs) const;

    //Settor
    void Zero(void);
    void Set(int xx=0.0f, int yy=0.0f, int zz=0.0f, int ww=1.0f);

    //Helpers
    void      Print(void) const;
    Vector4i  Min(const Vector4i& rhs);
    Vector4i  Max(const Vector4i& rhs);

    Vector4   to_f();
    std::string   to_string();


    int & operator[](int index);
    int operator[](int index) const;

    int x;
    int y;
    int z;
    int w;
  };  
}
