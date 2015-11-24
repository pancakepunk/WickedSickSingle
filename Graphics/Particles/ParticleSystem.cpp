#include "Precompiled.h"
#include "ParticleSystem.h"
#include "Particles/ParticleEmitter.h"

namespace WickedSick
{
  ParticleSystem::ParticleSystem(ParticleComponent * base) 
  : base_(base),
    active_(true)
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
    }
  }

  void ParticleSystem::Initialize()
  {
  }

  bool ParticleSystem::IsDead()
  {
    return time_passed_ > system_desc_.lifetime;
  }

  SystemDescription ParticleSystem::GetSystemDescription()
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

  std::vector<ParticleEmitter*>& ParticleSystem::GetEmitters()
  {
    return emitters_;
  }
}