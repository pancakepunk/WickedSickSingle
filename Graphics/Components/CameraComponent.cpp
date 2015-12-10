#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "CameraComponent.h"
#include "Input/InputInterface.h"

#include "ObjectFactory/ObjectFactoryInterface.h"


namespace WickedSick
{

  CameraComponent::CameraComponent() : Component(WickedSick::CT_CameraComponent)
  {
  }

  CameraComponent::~CameraComponent()
  {
  }

  void CameraComponent::Initialize()
  {

  }

  void CameraComponent::Update(float dt)
  {

  }

  void CameraComponent::SetRotation(const Vector3 & rot)
  {
    Transform* tr = static_cast<Transform*>(GetSibling(CT_Transform));
    tr->SetRotation(rot);
    //rotation_ = rot;
  }

  Vector3 CameraComponent::GetRotation()
  {
    Transform* tr = static_cast<Transform*>(GetSibling(CT_Transform));
    return tr->GetRotation();
  }

  void CameraComponent::SetLookAt(const Vector3& lookat)
  {
    look_at_ = lookat;
  }

  void CameraComponent::Move(const Vector3i & movement)
  {
    move_ = movement;
  }

  void CameraComponent::Clone(Component* source)
  {
    CameraComponent* camComp = (CameraComponent*)source;
    draw_dist_info_ = camComp->draw_dist_info_;
    look_at_ = camComp->look_at_;
    rotation_ = camComp->rotation_;
  }

  Vector3i CameraComponent::GetMovement()
  {
    auto toReturn = move_;
    move_.Zero();
    return toReturn;
  }

  Vector3 CameraComponent::GetLookAt()
  {
    return look_at_;
  }
}
