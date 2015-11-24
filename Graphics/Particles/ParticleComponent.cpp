#include "Precompiled.h"
#include "ParticleComponent.h"
#include "particles/ParticleSystem.h"
namespace WickedSick
{
  ParticleComponent::ParticleComponent() : Component(CT_ParticleComponent)
  {
  }
  
  ParticleComponent::~ParticleComponent()
  {
  }


  void ParticleComponent::Initialize()
  {
  }

  void ParticleComponent::Update(float dt)
  {
    for(auto& it : systems_)
    {
      it->Update(dt);
    }
  }

  void ParticleComponent::AddParticleSystem(ParticleSystem * newSys)
  {
    systems_.push_back(newSys);
    newSys->SetBase(this);
  }

  void ParticleComponent::RemoveParticleSystem(const std::string & name)
  {
    for(int i = 0; i < systems_.size(); ++i)
    {
      if(systems_[i]->GetSystemDescription().name == name)
      {
        vector_remove(systems_, i);
        return;
      }
    }
  }
}
