#pragma once
#include "Math/MathInterface.h"
#include "Component.h"



namespace WickedSick
{
  class GameObject;
#define MINSCALE 0.01f
  class Transform : public Component
  {
  public:
    MetaDef;
    Transform(GameObject*     owner = nullptr,
                          const Vector3&  pos    = Vector3(),
                          const Vector3&  scale  = Vector3(1.0f, 1.0f, 1.0f),
                          const Vector3&  rot    = Vector3());
    
    Transform(const Transform& rhs);

    ~Transform();

    void Initialize() override;
    void Update(float dt) override;
    void Clone(Component* source) override;


    Vector3 GetPosition();
    void SetPosition(const Vector3& pos);
    void SetPosition( float x, 
                                  float y, 
                                  float z);

    Vector3 GetRotation();
    void SetRotation(const Vector3& rot);
    void SetRotation( float x, 
                                  float y, 
                                  float z);

    Vector3 GetScale();
    void SetScale(const Vector3& scale);
    void SetScale(float x, 
                              float y, 
                              float z);
    void SetScale(float scale);


  private:
    const std::string name_;
    Vector3 pos_;
    Vector3 scale_;
    Vector3 rot_;
  };
}