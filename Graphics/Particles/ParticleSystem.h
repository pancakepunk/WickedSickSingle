#pragma once
#include "Math/MathInterface.h"
#include<vector>
namespace WickedSick
{
  class ParticleComponent;
  class ParticleEmitter;
  struct SystemDescription
  {
    MetaDef;
    float lifetime;
    Vector3 position;
    std::string name;
  };

  class ParticleSystem
  {
  public:
    MetaDef;
    ParticleSystem(ParticleComponent* base = nullptr);
    ~ParticleSystem();
    ParticleComponent* GetComponent();
    void AddEmitter(ParticleEmitter* particle);
    void RemoveEmitter(size_t index);
    void Initialize();
    bool IsDead();
    SystemDescription GetDescription();
    void SetSystemDescription(const SystemDescription& newDesc);
    void Update(float dt);
    void SetBase(ParticleComponent* newBase);
    void SetActive(bool active);
    bool GetActive();

    void Save(const std::string& filename);
    void Load(const std::string& filename);

    ParticleEmitter* GetEmitter(size_t index);
    std::vector<ParticleEmitter*>& GetEmitters();
  private:
    std::vector<ParticleEmitter*> emitters_;
    ParticleComponent* base_;
    SystemDescription  system_desc_;

    float time_passed_;
    bool active_;


  };
}
