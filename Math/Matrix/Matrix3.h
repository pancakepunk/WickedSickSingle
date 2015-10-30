#pragma once



namespace WickedSick
{
  struct Vector3;

  struct Matrix3
  {
    union
    {
      struct  
      {
        float m00, m01, m02,
              m10, m11, m12,
              m20, m21, m22;
      };
  
      float m[3][3];
      float v[9];
    };
  
    // Constructors
    Matrix3(void);
    Matrix3(const Matrix3& rhs);
    Matrix3(float mm00, float mm01, float mm02,
                        float mm10, float mm11, float mm12,
                        float mm20, float mm21, float mm22);
  
    // Assignment operator, does not need to handle self-assignment
    Matrix3& operator=(const Matrix3& rhs);
  
    // Multiplying a Matrix3 with a Vector3 or a Point3
    Vector3 operator*(const Vector3& rhs) const;
  
    // Basic Matrix arithmetic operations
    Matrix3 operator+(const Matrix3& rhs) const;
    Matrix3 operator-(const Matrix3& rhs) const;
    Matrix3 operator*(const Matrix3& rhs) const;
    Matrix3& operator+=(const Matrix3& rhs);
    Matrix3& operator-=(const Matrix3& rhs);
    Matrix3& operator*=(const Matrix3& rhs);
    Matrix3 operator*(const float rhs) const;
    Matrix3 operator/(const float rhs) const;
    Matrix3& operator*=(const float rhs);
    Matrix3& operator/=(const float rhs);
  
    // Comparison operators 
    bool operator==(const Matrix3& rhs) const;
    bool operator!=(const Matrix3& rhs) const;

    // Linear Algebra
    void Transpose();
    void Inverse();

    //rotation
    void MakeRotateX(float x);
    void MakeRotateY(float y);
    void MakeRotateZ(float z);

    // Settors
    void Zero(void);
    void Identity(void);
  
    // Other
    void Print(void) const;
  };
}

