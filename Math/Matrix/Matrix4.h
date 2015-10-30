#pragma once



namespace WickedSick
{
  struct Vector3;
  struct Vector4;

  struct Matrix4
  {
    union
    {
      struct  
      {
        float m00, m01, m02, m03,
              m10, m11, m12, m13,
              m20, m21, m22, m23,
              m30, m31, m32, m33;
      };
  
      float m[4][4];
      float v[16];
    };
  
    // Constructor
    Matrix4(void);
    Matrix4(const Matrix4& rhs);
    Matrix4(const float* a);
    Matrix4(float mm00, float mm01, float mm02, float mm03,
                        float mm10, float mm11, float mm12, float mm13,
                        float mm20, float mm21, float mm22, float mm23,
                        float mm30, float mm31, float mm32, float mm33);
  
    // Assignment operator, does not need to handle self-assignment
    Matrix4& operator=(const Matrix4& rhs);
  
    // Multiplying a Matrix4 with a Vector4 or a Point4
    Vector4 operator*(const Vector4& rhs) const;
  
    // Basic Matrix arithmetic operations
    Matrix4 operator+(const Matrix4& rhs) const;
    Matrix4 operator-(const Matrix4& rhs) const;
    Matrix4 operator*(const Matrix4& rhs) const;
    Matrix4& operator+=(const Matrix4& rhs);
    Matrix4& operator-=(const Matrix4& rhs);
    Matrix4& operator*=(const Matrix4& rhs);
    Matrix4 operator*(const float rhs) const;
    Matrix4 operator/(const float rhs) const;
    Matrix4& operator*=(const float rhs);
    Matrix4& operator/=(const float rhs);
  
    // Comparison operators
    bool operator==(const Matrix4& rhs) const;
    bool operator!=(const Matrix4& rhs) const;

    // Linear Algebra
    Matrix4 GetTranspose() const;
    void Transpose();
    void Inverse();

    // Settors
    Matrix4& Zero(void);
    Matrix4& Identity(void);
  
    Matrix4& ScaleMat(const Vector3& scale);
    Matrix4 GetScaled(const Vector3& scale) const;

    Matrix4& RotateX(float x);
    Matrix4& RotateY(float y);
    Matrix4& RotateZ(float z);
    Matrix4 GetRotatedX(float x) const;
    Matrix4 GetRotatedY(float y) const;
    Matrix4 GetRotatedZ(float z) const;

    Matrix4& RotateXYZ(const Vector3& rot);
    Matrix4 GetRotatedXYZ(const Vector3& rot) const;

    Matrix4& Translate(const Vector3& dist);
    Matrix4 GetTranslated(const Vector3& dist) const;

    // Utility

    

    // Other
    void Print(void) const;


    //graphics
    Matrix4& DoPerspective(float fovW, float znear, float zfar, float aspectRatio);
    Matrix4& DoOrthographic(int width, int height, float znear, float zfar);
  };
}

