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
      else //the particle systems are updated via the components
      {
        //if(particle_systems_[i]->GetActive())
        //{
        //  particle_systems_[i]->Update(dt);
        //}
        ++i;
      }
      
    }
  }

  ParticleSystem * ParticleManager::MakeParticleSystem(const SystemDescription & desc)
  {
    auto newSystem = system_manager_.New();
    newSystem->SetSystemDescription(desc);
    newSystem->Update(0.0f);
    //particle_systems_.push_back(newSystem);
    return newSystem;
  }

  ParticleEmitter* ParticleManager::MakeParticleEmitter(const EmitterDescription& desc)
  {
    auto newEmitter = emitter_manager_.New(desc);
    newEmitter->Update(0.0f);
    return newEmitter;
  }

  ParticleSystem* ParticleManager::CloneParticleSystem(ParticleSystem* source)
  {
    ParticleSystem* toReturn = MakeParticleSystem(source->GetDescription());
    ParticleEmitter* toAdd;
    for(auto& peit : source->GetEmitters())
    {
      toReturn->AddEmitter(CloneParticleEmitter(peit));
    }
    return toReturn;
  }

  ParticleEmitter* ParticleManager::CloneParticleEmitter(ParticleEmitter* source)
  {
    ParticleEmitter* toReturn = MakeParticleEmitter(source->GetDescription());
    auto& states = source->GetStates();
    auto& variance = source->GetStateVariances();
    for(int i = 0; i < states.size(); ++i)
    {
      toReturn->AddParticleState(states[i], variance[i]);
    }
    auto& attributes = source->GetAttributes();
    for(auto& it : attributes)
    {
      toReturn->RegisterAttribute(it);
    }
    return toReturn;
  }

  void ParticleManager::DeleteSystem(ParticleSystem * target)
  {
    for(int i = 0; i < particle_systems_.size(); ++i)
    {
      if(particle_systems_[i] == target)
      {
        vector_remove(particle_systems_, i);
        break;
      }
    }
    system_manager_.Delete(target);
  }

  void ParticleManager::DeleteEmitter(ParticleEmitter * target)
  {
    emitter_manager_.Delete(target);
  }

  ParticleSystem * ParticleManager::GetSystem(size_t index)
  {
    if(index < particle_systems_.size())
    {
      return particle_systems_[index];
    }
    return nullptr;
  }

  std::vector<ParticleSystem*>& ParticleManager::GetSystems()
  {
    return particle_systems_;
  }

}