#pragma once



namespace WickedSick
{
  struct Vector2;

  struct Matrix2
  {
    union
    {
      struct  
      {
        float m00, m01,
              m10, m11;
      };
  
      float m[2][2];
      float v[4];
    };
  
    // Constructors
    Matrix2(void);
    Matrix2(const Matrix2& rhs);
    Matrix2(float mm00, float mm01,
            float mm10, float mm11);
  
    // Assignment operator, does not need to handle self-assignment
    Matrix2& operator=(const Matrix2& rhs);
  
    // Multiplying a Matrix2 with a Vector2 or a Point2
    Vector2 operator*(const Vector2& rhs) const;
  
    // Basic Matrix arithmetic operations
    Matrix2 operator+(const Matrix2& rhs) const;
    Matrix2 operator-(const Matrix2& rhs) const;
    Matrix2 operator*(const Matrix2& rhs) const;
    Matrix2& operator+=(const Matrix2& rhs);
    Matrix2& operator-=(const Matrix2& rhs);
    Matrix2& operator*=(const Matrix2& rhs);
    Matrix2 operator*(const float rhs) const;
    Matrix2 operator/(const float rhs) const;
    Matrix2& operator*=(const float rhs);
    Matrix2& operator/=(const float rhs);
  
    // Comparison operators
    bool operator==(const Matrix2& rhs) const;
    bool operator!=(const Matrix2& rhs) const;

    // Linear Algebra
    void Transpose(void);
    void Inverse(void);

    // Settors
    void Zero(void);
    void Identity(void);
  
   // Other
    void Print(void) const;
  };
}
