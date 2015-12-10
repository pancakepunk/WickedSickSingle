#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "Camera.h"
#include "Components/CameraComponent.h"

namespace WickedSick
{
  Camera::Camera() 
  : source_(nullptr),
    dirty_(true)
  {

  }

  Camera::~Camera()
  {

  }

  Vector3 Camera::GetPosition()
  {
    if(source_)
    {
      return static_cast<Transform*>(source_->GetSibling(CT_Transform))->GetPosition();
    }

    return pos_;

    
  }

  void Camera::SetPosition(const Vector3& pos)
  {
    if(source_)
    {
      static_cast<Transform*>(source_->GetSibling(CT_Transform))->SetPosition(pos);
    }
    else
    {
      pos_ = pos;;
    }
  }

  void Camera::SetPosition(float x, float y, float z)
  {
    SetPosition(Vector3(x, y, z));
  }

  Vector3 Camera::GetRotation()
  {
    if(source_)
    {
      return static_cast<Transform*>(source_->GetSibling(CT_Transform))->GetRotation();
    }
    return rot_;
  }

  void Camera::SetRotation(const Vector3& rot)
  {
    if(source_)
    {
      static_cast<Transform*>(source_->GetSibling(CT_Transform))->SetRotation(rot);
    }
    else
    {
      rot_ = rot;
    }
    
  }

  void Camera::SetRotation(float x, float y, float z)
  {
    rot_ = Vector3(x, y, z);
  }

  //deprecated
  Vector3 Camera::GetLookAt()
  {
    if (source_)
    {
      return source_->GetLookAt();
    }

    return look_at_;

  }

  void Camera::Orient()
  {
    Vector3 upVector = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 lookAt;
    Vector3 pos;
    Vector3 rotation;
    Vector3i toMove;
    Transform* tr = nullptr;
    if(source_)
    {
      lookAt = source_->GetLookAt();
      tr = static_cast<Transform*>(source_->GetSibling(CT_Transform));
      rotation = tr->GetRotation();
      toMove = source_->GetMovement();
      pos = tr->GetPosition();
    }
    else // default
    {
      lookAt.Zero();
      
      pos.z = -5.0f;
      pos.y = 5.0f;
    }


    if(upVector == look_at_.GetNormalized())
    {
      upVector = Vector3(0.1f, 1.0f, 0.0f);
    }
    //rotation = rot_;
    rotation *= 0.0174532925f;

    //we'd use this so we can rotate our axis. not relevant right now.
    Matrix4 mx;
    Matrix4 my;
    mx.Identity();
    my.Identity();
    mx.RotateX(rotation.x);
    my.RotateY(rotation.y);
    Matrix4 rotationMat = mx * my;
    //rotationMat.RotateZ(rot_.z);
    Vector3 up = Vector3(0.0f, 1.0f, 0.0f) * rotationMat;
    //Vector3 right = Vector3(1.0f, 0.0f, 0.0f) * rotationMat;

    Vector3 view = Vector3(0.0f, 0.0f, 1.0f) * rotationMat;
    Vector3 right = up.Cross(view).GetNormalized();
    pos -= up * toMove.y;
    pos += right * toMove.x;
    pos += view * toMove.z;
    if(tr)
    {
      tr->SetPosition(pos);

    }


    //wtf is this for?
    //look_at_ = rotationMat * look_at_;

    //why am i doing this?
    //look_at_ += pos_;
    pos_ = pos;

    view_.Identity();
    
    view_.m[0][0] = right.x;  view_.m[0][1] = up.x;  view_.m[0][2] = view.x;  view_.m[0][3] = 0.0f;
    view_.m[1][0] = right.y;  view_.m[1][1] = up.y;  view_.m[1][2] = view.y;  view_.m[1][3] = 0.0f;
    view_.m[2][0] = right.z;  view_.m[2][1] = up.z;  view_.m[2][2] = view.z;  view_.m[2][3] = 0.0f;
    view_.m[3][0] = -right.Dot(pos);     view_.m[3][1] = -up.Dot(pos);     view_.m[3][2] = -view.Dot(pos);     view_.m[3][3] = 1.0f;
    view_.Transpose();
    //return;
    //view_.m[0][0] = xaxis.x;  view_.m[0][1] = xaxis.y;  view_.m[0][2] = xaxis.z;  view_.m[0][3] = -xaxis.Dot(pos);
    //view_.m[1][0] = yaxis.x;  view_.m[1][1] = yaxis.y;  view_.m[1][2] = yaxis.z;  view_.m[1][3] = -yaxis.Dot(pos);
    //view_.m[2][0] = -zaxis.x;  view_.m[2][1] = -zaxis.y;  view_.m[2][2] = -zaxis.z;  view_.m[2][3] = -zaxis.Dot(pos);
    //view_.m[3][0] = 0.0f;     view_.m[3][1] = 0.0f;     view_.m[3][2] = 0.0f;     view_.m[3][3] = 1.0f;
    //view_.Transpose();
    //view_.Translate(Vector3(-pos_.x, -pos_.y, -pos_.z));

  }

  void Camera::SetSource(CameraComponent * source)
  {
    source_ = source;
  }

  Matrix4 Camera::GetViewMatrix()
  {
    return view_;
  }

  void Camera::Clean()
  {
    dirty_ = false;
  }

  void Camera::Dirty()
  {
    dirty_ = true;
  }

  bool Camera::IsDirty()
  {
    return dirty_;
  }
}