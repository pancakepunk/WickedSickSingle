#pragma once
#include "Utility/UtilityInterface.h"
#include "ParticleDescription.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
namespace WickedSick
{
  class ParticleManager
  {
  public:
    ParticleManager();

    void Update(float dt);
    ParticleSystem* MakeParticleSystem(const SystemDescription& desc);
    ParticleEmitter* MakeParticleEmitter(size_t count, const EmitterDescription& desc);
    std::vector<ParticleSystem*>& GetSystems();
  private:
    std::vector<ParticleSystem*> particle_systems_;
    MemoryManager<ParticleEmitter> emitter_manager_;
    MemoryManager<ParticleSystem> system_manager_;
  };
 
}