#pragma once
#include "Meta/MetaInterface.h"


namespace WickedSick
{
  struct Vector2i;

  struct Vector2
  {
    MetaDef;
    // Constructor
    Vector2(void);  
    Vector2(const Vector2& rhs);

    Vector2(float xx, float yy);
  
    // Assignment operator, does not need to handle self assignment
    Vector2 & operator=(const Vector2& rhs);
  
    // Unary negation operator, negates all components and returns a copy
    Vector2   operator-(void) const;  
    
    // Math Operators
    Vector2 operator+(const Vector2& rhs) const;
    Vector2 operator-(const Vector2& rhs) const;
    Vector2 operator*(const float rhs) const;
    Vector2 operator/(const float rhs) const;
    Vector2 & operator+=(const Vector2& rhs);
    Vector2 & operator-=(const Vector2& rhs);
    Vector2 & operator*=(const float rhs);
    Vector2 & operator/=(const float rhs);
  
    // Comparison operators
    bool      operator==(const Vector2& rhs) const;
    bool      operator!=(const Vector2& rhs) const;
         
    // Casting operators
    friend Vector2 operator*(float f, Vector2 const & v);

    // Linear Algebra
    float     Length(void) const;
    float     LengthSq(void) const;
    void      Negate(void);
    Vector2   GetNormalized(void);
    void      Normalize(void);
    float     Dot(const Vector2& rhs) const;
    float     Cross(const Vector2& rhs) const;
    bool      IsZero(void);

    // set all components to zero
    void      Zero(void);
    void      Set(float xx=0.0f, float yy=0.0f);

    // Helpers
    void      Print(void) const;
    Vector2   Min(const Vector2& rhs);
    Vector2   Max(const Vector2& rhs);

    Vector2i  to_i();
    std::string to_string();

    float  & operator[](int index);
    float   operator[](int index) const;
    
    float x;
    float y;
  };
}
