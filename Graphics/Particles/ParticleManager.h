#pragma once
#include "Utility/UtilityInterface.h"
#include "ParticleDescription.h"
#include "ParticleEmitter.h"
namespace WickedSick
{
  class ParticleManager
  {
  public:
    ParticleManager();

    void Update(float dt);

    ParticleEmitter* MakeParticleEmitter(size_t count, const EmitterDescription& desc);
    void KillParticleEmitter(ParticleEmitter* sys);
    std::vector<ParticleEmitter*>& GetEmitters();
  private:
    std::vector<ParticleEmitter*> particle_emitters_;
    MemoryManager<ParticleEmitter> emitter_manager_;
  };
 
}