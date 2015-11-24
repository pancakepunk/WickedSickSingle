#include "Precompiled.h"
#include "ParticleManager.h"

namespace WickedSick
{
  ParticleManager::ParticleManager()
  {
    //    ParticleSystem::basePtr = this;
  }

  void ParticleManager::Update(float dt)
  {
    for(size_t i = 0; i < particle_systems_.size(); )
    {
      if(particle_systems_[i]->IsDead())
      {
        system_manager_.Delete(particle_systems_[i]);
        vector_remove(particle_systems_, i);
      }
      else 
      {
        if(particle_systems_[i]->GetActive())
        {
          particle_systems_[i]->Update(dt);
        }
        ++i;
      }
      
    }
  }

  ParticleSystem * ParticleManager::MakeParticleSystem(const SystemDescription & desc)
  {
    auto newSystem = system_manager_.New();
    newSystem->SetSystemDescription(desc);
    newSystem->Update(0.0f);
    particle_systems_.push_back(newSystem);
    return newSystem;
  }

  ParticleEmitter* ParticleManager::MakeParticleEmitter(size_t count, const EmitterDescription& desc)
  {
    auto newEmitter = emitter_manager_.New(count, desc);
    newEmitter->Update(0.0f);
    return newEmitter;
  }

  std::vector<ParticleSystem*>& ParticleManager::GetSystems()
  {
    return particle_systems_;
  }

}