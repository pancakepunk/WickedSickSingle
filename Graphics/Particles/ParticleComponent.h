#pragma once

#include "Core/CoreInterface.h"

namespace WickedSick
{
  class ParticleSystem;
  class ParticleComponent : public Component
  {
  public:
    ParticleComponent();
    ~ParticleComponent();
    void Initialize() override;
    void Update(float dt) override;
    void AddParticleSystem(ParticleSystem* newSys);
    void RemoveParticleSystem(const std::string& name);
  private:
    std::vector<ParticleSystem*> systems_;
  };
}
