#include "Precompiled.h"
#include "ParticleComponent.h"
#include "Core/CoreInterface.h"
#include "Graphics/GraphicsInterface.h"
#include "particles/ParticleSystem.h"
namespace WickedSick
{
  ParticleComponent::ParticleComponent() : Component(CT_ParticleComponent)
  {
  }
  
  ParticleComponent::~ParticleComponent()
  {
  }

  ParticleComponent::ParticleComponent(const ParticleComponent & rhs) : Component(CT_ParticleComponent)
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

  void ParticleComponent::Clone(Component * source)
  {
    //need to clone all our systems
    ParticleComponent* particleComp = (ParticleComponent*)source;
    Graphics* gSys = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    auto particleManager = gSys->GetParticleManager();
    ParticleSystem* curSys;
    ParticleEmitter* curEmitter;
    for(auto& psit : particleComp->systems_)
    {
      curSys = particleManager->CloneParticleSystem(psit);
      AddParticleSystem(curSys);
      curSys->Initialize();
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
      if(systems_[i]->GetDescription().name == name)
      {
        Graphics* graphics = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
        ParticleManager* manager = graphics->GetParticleManager();
        manager->DeleteSystem(systems_[i]);
        vector_remove(systems_, i);
        return;
      }
    }
  }

  void ParticleComponent::RemoveParticleSystem(size_t index)
  {
    if((index < systems_.size()) && (index > 0))
    {
      Graphics* graphics = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
      ParticleManager* manager = graphics->GetParticleManager();
      manager->DeleteSystem(systems_[index]);
      vector_remove(systems_, index);
    }
  }
  std::vector<ParticleSystem*>& ParticleComponent::GetSystems()
  {
    return systems_;
  }
}
