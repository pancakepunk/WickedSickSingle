#include "Precompiled.h"
#include "ReflectComponent.h"
#include "Graphics/GraphicsInterface.h"
namespace WickedSick
{
  ReflectComponent::ReflectComponent() 
  : Component(CT_ReflectComponent, nullptr),
    reflect_ratio_(1.0),
    refract_ratio_(1.0)
  {
  }

  ReflectComponent::~ReflectComponent()
  {
  }

  void ReflectComponent::Initialize()
  {
  }

  void ReflectComponent::Update(float dt)
  {
  }

  void ReflectComponent::Clone(Component * source)
  {
    ReflectComponent* reflect = (ReflectComponent*)source;
    reflect_ratio_ = reflect->reflect_ratio_;
    refract_ratio_ = reflect->refract_ratio_;
  }

  float ReflectComponent::GetReflectionCoeff()
  {
    return reflect_ratio_;
  }

  void ReflectComponent::SetReflectionCoeff(float ref)
  {
    reflect_ratio_ = ref;
  }

  float ReflectComponent::GetRefractionCoeff()
  {
    return refract_ratio_;
  }
  void ReflectComponent::SetRefractionCoeff(float ref)
  {
    refract_ratio_ = ref;
  }

}

RegisterType(WickedSick, ReflectComponent)
RegisterMember(reflect_ratio_);
RegisterMember(refract_ratio_);
}