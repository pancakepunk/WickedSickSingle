#include "Precompiled.h"
#include "ParticleSystem.h"
#include "Particles/ParticleEmitter.h"
#include "Graphics/GraphicsInterface.h"


namespace WickedSick
{
  ParticleSystem::ParticleSystem(ParticleComponent * base) 
  : base_(base),
    active_(false)
  {
    time_passed_ = 0.0f;
  }

  ParticleSystem::~ParticleSystem()
  {
  }

  ParticleComponent* ParticleSystem::GetComponent()
  {
    return base_;
  }

  void ParticleSystem::AddEmitter(ParticleEmitter* particle)
  {
    if(particle)
    {
      emitters_.push_back(particle);
      particle->SetBase(this);
    }
  }

  void ParticleSystem::RemoveEmitter(size_t index)
  {
    Graphics* graphics = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    ParticleManager* manager = graphics->GetParticleManager();
    
    manager->DeleteEmitter(emitters_[index]);
    vector_remove(emitters_, index);
  }

  //this is where we add it to the particle manager because fuck having archetypes in the global list
  void ParticleSystem::Initialize()
  {
    Graphics* gSys = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    
    ParticleManager* manager = gSys->GetParticleManager();
    manager->GetSystems().push_back(this);
    active_ = true;

    //add a single emitter
    AddEmitter(manager->MakeParticleEmitter());
  }

  bool ParticleSystem::IsDead()
  {
    return time_passed_ > system_desc_.lifetime;
  }

  SystemDescription ParticleSystem::GetDescription()
  {
    return system_desc_;
  }

  void ParticleSystem::SetSystemDescription( const SystemDescription& newDesc)
  {
    system_desc_ = newDesc;

  }

  void ParticleSystem::Update(float dt)
  {
    
    if(time_passed_ <= system_desc_.lifetime)
    {
      if(system_desc_.lifetime)
      {
        time_passed_ += dt;
      }
      for(auto it : emitters_)
      {
        it->Update(dt);
      }
    }
  }

  void ParticleSystem::SetBase(ParticleComponent * newBase)
  {
    base_ = newBase;
  }

  void ParticleSystem::SetActive(bool active)
  {
    active_ = active;
  }

  bool ParticleSystem::GetActive()
  {
    return active_;
  }

  void ParticleSystem::Save(const std::string& filename)
  {
    rapidxml::
  }

  ParticleEmitter * ParticleSystem::GetEmitter(size_t index)
  {
    if(index < emitters_.size())
    {
      return emitters_[index];
    }
    return nullptr;
  }

  std::vector<ParticleEmitter*>& ParticleSystem::GetEmitters()
  {
    return emitters_;
  }
}

RegisterType(WickedSick, SystemDescription)
RegisterMember(position);
RegisterMember(lifetime);
}

RegisterType(WickedSick, ParticleSystem)
RegisterMember(active_);
RegisterMember(system_desc_);
}