#pragma once


#include "meta/MetaInterface.h"
namespace WickedSick
{
  struct Vector2;
  struct Vector3;
  struct Vector4i;
  struct Matrix4;

  struct Vector4
  {
    MetaDef;
    // Constructors
    Vector4(void);
    Vector4(const Vector2& rhs, float zz=0.0f, float ww=1.0f);
    Vector4(const Vector3& rhs, float ww=1.0f);
    Vector4(const Vector4& rhs);
    Vector4(float xx, float yy, float zz, float ww);

    // Assignment operator, does not need to handle self assignment
    Vector4 & operator=(const Vector4& rhs);

    // Unary negation operator, negates all components and returns a copy
    Vector4 operator-(void) const;  

    // Math Operators
    Vector4 operator+(const Vector4& rhs) const;
    Vector4 operator-(const Vector4& rhs) const;
    Vector4 operator*(const Matrix4& rhs) const; 
    Vector4 operator*(const Vector4& rhs) const;
    Vector4 operator*(const float rhs) const;
    Vector4 operator/(const float rhs) const;
    Vector4 & operator+=(const Vector4& rhs);
    Vector4 & operator-=(const Vector4& rhs);
    Vector4 & operator*=(const float rhs);
    Vector4 & operator/=(const float rhs);

    // Comparison operators
    bool operator==(const Vector4& rhs) const;
    bool operator!=(const Vector4& rhs) const;
         
    // casting operators
    friend Vector4 operator*(float f, Vector4 const & v);

    // Linear Algebra
    float   Length(void) const;
    float   LengthSq(void) const;
    void    Negate(void);
    void    Maximize(void);
    void    Minimize(void);
    Vector4 GetNormalized(void); //returns a normalized version of the vector
    void    Normalize(void);
    float   Dot(const Vector4& rhs) const;
    Vector4 Cross(const Vector4& rhs) const;
    bool    IsZero(void);

    //Settor
    void Zero(void);
    void Set(float xx=0.0f, float yy=0.0f, float zz=0.0f, float ww=1.0f);

    //Helpers
    void    Print(void) const;
    Vector4 Min(const Vector4& rhs);
    Vector4 Max(const Vector4& rhs);

    Vector4i to_i();
    Vector3 xyz();
    Vector2 xy();
    std::string to_string();

    float & operator[](int index);
    float   operator[](int index) const;


    float x;
    float y;
    float z;
    float w;
  };  
}
