#include "ParticleManager.h"

namespace WickedSick
{
  ParticleManager::ParticleManager()
  {
    //    ParticleSystem::basePtr = this;
  }

  void ParticleManager::Update(float dt)
  {
    for(size_t i = 0; i < particle_emitters_.size(); )
    {
      if(particle_emitters_[i]->IsDead())
      {
        emitter_manager_.Delete(particle_emitters_[i]);
        vector_remove(particle_emitters_, i);
      }
      else
      {
        particle_emitters_[i]->Update(dt);
        ++i;
      }
      
    }
  }

  ParticleEmitter* ParticleManager::MakeParticleEmitter(size_t count, const EmitterDescription& desc)
  {
    particle_emitters_.push_back(emitter_manager_.New(count, desc));
    for(size_t i = 0; i < particle_emitters_.size(); ++i)
    {
      particle_emitters_[i]->Update(0.0f);
    }
    return particle_emitters_.back();
  }

  void ParticleManager::KillParticleEmitter(ParticleEmitter * emitter)
  {
    emitter->Kill();
  }

  std::vector<ParticleEmitter*>& ParticleManager::GetEmitters()
  {
    return particle_emitters_;
  }

}