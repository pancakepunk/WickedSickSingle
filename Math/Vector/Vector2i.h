#pragma once



namespace WickedSick
{
  struct Vector2;

  struct Vector2i
  {

    // Constructor
    Vector2i(void);  
    Vector2i(const Vector2i& rhs);
    Vector2i(int xx, int yy);
  
    // Assignment operator, does not need to handle self assignment
    Vector2i & operator=(const Vector2i& rhs);
  
    // Unary negation operator, negates all components and returns a copy
    Vector2i operator-(void) const;  
    
    // Math Operators
    Vector2i operator+(const Vector2i& rhs) const;
    Vector2i operator-(const Vector2i& rhs) const;
    Vector2i operator*(const int rhs) const;
    Vector2i operator/(const int rhs) const;
    Vector2i & operator+=(const Vector2i& rhs);
    Vector2i & operator-=(const Vector2i& rhs);
    Vector2i & operator*=(const int rhs);
    Vector2i & operator/=(const int rhs);
  
    // Comparison operators
    bool operator==(const Vector2i& rhs) const;
    bool operator!=(const Vector2i& rhs) const;
         
    // Linear Algebra
    float    Length(void) const;
    int      LengthSq(void) const;
    void     Negate(void);
    Vector2  GetNormalized(void) const;
    int      Dot(const Vector2i& rhs) const;
    int      Cross(const Vector2i& rhs) const;

    void Zero(void);
    void Set(int xx=0, int yy=0);

    // Helpers
    void      Print(void) const;
    Vector2i  Min(const Vector2i& rhs);
    Vector2i  Max(const Vector2i& rhs);

    Vector2 to_f() const;
    std::string to_string() const;

    int & operator[](int index);
    int operator[](int index) const;

    int x; 
    int y;
  };
}
