#pragma once
#include "ParticleEnums.h"
//#include "ParticleDescriptor.h"
#include "Interpolator.h"
#include "Meta/MetaInterface.h"
#include "Core/CoreInterface.h"
namespace WickedSick
{

  class InterpolatorInterface;
  class Model;
  class Texture;
  struct EmitterDescription
  {
    std::string sourceModel;
    std::string sourceTexture;
    Vector3 spawnPos;
    size_t amountToSpawn;
    float frequency;// frequency of bursts
    float emitLength;//amount of time each burst lasts
    //note: in order to have a typical particle stream, 
    //      make sure that either frequency and emitLength are both low
    //      or set emitLength to zero and frequency to a low number

    double lifetime;
    bool lazy;

  };

  class ParticleEmitter
  {
  public:
    ParticleEmitter(size_t particleCount,
                    const EmitterDescription& description);
    void Update(float dt);

    EmitterDescription& GetEmitterDescription();
    bool IsDead();
    void Kill();

    void Emit(size_t count);

    void AddParticleState(const ParticleDescription& particle,
                          const ParticleDescription& variance = ParticleDescription());
    void ClearParticleStates();
    void RegisterAttribute(const std::string& memberName);

    void Render();
    std::set<size_t>& GetAlive();
    std::vector<ParticleDescription>& GetParticles();
//      static ParticleManager* basePtr;
  private:
      
    friend class ParticleInterpolator;

    void init_particle(ParticleDescription& particle, size_t index);

    float burst_timer_;
    float emit_timer_;
    EmitterDescription description_;
    std::set<size_t> live_;
    std::set<size_t> dead_;
    std::vector<ParticleDescription> particles_;

    std::vector<ParticleDescription> states_;
    std::vector<ParticleDescription> variance_;

    std::vector<ParticleInterpolator> interpolators_;

    std::set<Reflection::Member*> members_to_interpolate_;

  };

  class ParticleSystem
  {
  public:
    ParticleSystem();
    ~ParticleSystem();
    void AddEmitter(const ParticleEmitter& particle);

  private:
    std::vector<ParticleEmitter> emitters_;
  };
  
}
