#pragma once

#include "Core/CoreInterface.h"

namespace WickedSick
{

  class ReflectComponent : public Component
  {
  public:
    MetaDef;
    ReflectComponent();
    ~ReflectComponent();
    void Initialize() override;
    void Update(float dt) override;
    void Clone(Component* source) override;


    float GetReflectionCoeff();
    void SetReflectionCoeff(float ref);
    float GetRefractionCoeff();
    void SetRefractionCoeff(float ref);
  private:
    float reflect_ratio_;
    float refract_ratio_;
  };

}
