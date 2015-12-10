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
    ParticleComponent(const ParticleComponent& rhs);
    void Initialize() override;
    void Update(float dt) override;
    void Clone(Component* source) override;
    void AddParticleSystem(ParticleSystem* newSys);
    void RemoveParticleSystem(const std::string& name);
    void RemoveParticleSystem(size_t index);
    std::vector<ParticleSystem*>& GetSystems();

  private:
    std::vector<ParticleSystem*> systems_;
  };
}
