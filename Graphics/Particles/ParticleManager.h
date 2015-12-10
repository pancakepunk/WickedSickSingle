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
    ParticleSystem* MakeParticleSystem(const SystemDescription& desc = SystemDescription());
    ParticleEmitter* MakeParticleEmitter(const EmitterDescription& desc = EmitterDescription());
    ParticleSystem* CloneParticleSystem(ParticleSystem* source);
    ParticleEmitter* CloneParticleEmitter(ParticleEmitter* source);

    void DeleteSystem(ParticleSystem* target);
    void DeleteEmitter(ParticleEmitter* target);

    ParticleSystem* GetSystem(size_t index);
    std::vector<ParticleSystem*>& GetSystems();
  private:
    std::vector<ParticleSystem*> particle_systems_;
    MemoryManager<ParticleEmitter> emitter_manager_;
    MemoryManager<ParticleSystem> system_manager_;
  };
 
}