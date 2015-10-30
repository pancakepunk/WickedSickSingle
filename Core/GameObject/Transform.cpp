#include "CorePrecompiled.h"
#include "Transform.h"

namespace WickedSick
{
  Transform::Transform( GameObject*     owner,
                                    const Vector3&  pos,
                                    const Vector3&  scale,
                                    const Vector3&  rot)
                                    : pos_(pos),
                                      scale_(scale),
                                      rot_(rot),
                                      name_("Transform"),
                                      Component(CT_Transform, owner)
  {

  }
    
  Transform::Transform( const Transform& rhs)
                                    : pos_(rhs.pos_),
                                      scale_(rhs.scale_),
                                      rot_(rhs.rot_),
                                      name_("Transform"),
                                      Component(CT_Transform)
  {

  }

  Transform::~Transform()
  {

  }

  void Transform::Initialize()
  {

  }

  void Transform::Update(float dt)
  {
    //nothing
  }

  Vector3 Transform::GetPosition()
  {
    return pos_;
  }

  void Transform::SetPosition(const Vector3& pos)
  {
    pos_ = pos;
  }

  void Transform::SetPosition(float x, float y, float z)
  {
    pos_ = Vector3(x, y, z);
  }


  Vector3 Transform::GetRotation()
  {
    return rot_;
  }

  void Transform::SetRotation(const Vector3& rot)
  {
    rot_ = rot;
  }

  void Transform::SetRotation(float x, float y, float z)
  {
    rot_ = Vector3(x, y, z);
  }

  Vector3 Transform::GetScale()
  {
    return scale_;
  }

  void Transform::SetScale(const Vector3& scale)
  {
    scale_.x = std::max( MINSCALE, scale.x);
    scale_.y = std::max( MINSCALE, scale.y);
    scale_.z = std::max( MINSCALE, scale.z);
  }

  void Transform::SetScale(float x, float y, float z)
  {
    scale_.x = std::max( MINSCALE, x);
    scale_.y = std::max( MINSCALE, y);
    scale_.z = std::max( MINSCALE, z);
  }

  void Transform::SetScale(float scale)
  {
    scale_.x = scale_.y = scale_.z = std::max( MINSCALE, scale);
  }

}


RegisterType(WickedSick, Transform)
RegisterMember(pos_);
RegisterMember(scale_);
RegisterMember(rot_);
}