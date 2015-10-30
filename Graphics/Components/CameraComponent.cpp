#include "GraphicsPrecompiled.h"
#include "CameraComponent.h"
#include "Input/InputInterface.h"

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

  void CameraComponent::SetLookAt(const Vector3& lookat)
  {
    look_at_ = lookat;
  }

  Vector3 CameraComponent::GetLookAt()
  {
    return look_at_;
  }
}
