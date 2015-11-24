#pragma once
#include "Math/MathInterface.h"
#include<vector>
namespace WickedSick
{
  class ParticleComponent;
  class ParticleEmitter;
  struct SystemDescription
  {
    float lifetime;
    Vector4 position;
    std::string name;
  };

  class ParticleSystem
  {
  public:
    ParticleSystem(ParticleComponent* base = nullptr);
    ~ParticleSystem();
    ParticleComponent* GetComponent();
    void AddEmitter(ParticleEmitter* particle);
    void Initialize();
    bool IsDead();
    SystemDescription ParticleSystem::GetSystemDescription();
    void ParticleSystem::SetSystemDescription(const SystemDescription& newDesc);
    void Update(float dt);
    void SetBase(ParticleComponent* newBase);
    void SetActive(bool active);
    bool GetActive();

    std::vector<ParticleEmitter*>& GetEmitters();
  private:
    std::vector<ParticleEmitter*> emitters_;
    ParticleComponent* base_;
    SystemDescription  system_desc_;

    float time_passed_;
    bool active_;


  };
}
