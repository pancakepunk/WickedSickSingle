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
  class ParticleSystem;
  namespace VelocityType
  {
    enum Enum
    {
      WorldSpace,
      EmitterSpace,
      Outward
    };
  }
  struct EmitterDescription
  {
    MetaDef;
    EmitterDescription();
    std::string sourceModel;
    std::string sourceTexture;
    Vector3 spawnPos;
    int amountToSpawn;
    int maxParticles;
    float frequency;// frequency of bursts
    float emitLength;//amount of time each burst lasts
    Vector3 upVector;
    //note: in order to have a typical particle stream, 
    //      make sure that either frequency and emitLength are both low
    //      or set emitLength to zero and frequency to a low number

    float lifetime;
    bool lazy;

  };
  //class Member;
  class ParticleEmitter
  {
  public:
    MetaDef;
    ParticleEmitter(const EmitterDescription& description,
                    ParticleSystem* base = nullptr);
    ParticleEmitter(const ParticleEmitter& rhs);
    void Update(float dt);

    EmitterDescription& GetDescription();
    bool IsDead();
    void Kill();

    


    void Emit(size_t count);

    void AddParticleState(const ParticleDescription& particle = ParticleDescription(),
                          const ParticleDescription& variance = ParticleDescription());
    void RemoveParticleState(size_t index);
    void ClearParticleStates();
    
    std::vector<ParticleDescription>& GetStates();
    std::vector<ParticleDescription>& GetStateVariances();


    void RegisterAttribute(const std::string& memberName);
    void RegisterAttribute(Reflection::Member* member);
    void UnregisterAttribute(const std::string& memberName);
    void UnregisterAttribute(Reflection::Member* member);

    std::set<Reflection::Member*>& GetAttributes();

    ParticleSystem* GetSystem();
    void SetBase(ParticleSystem* newBase);
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

    ParticleSystem* base_;

  };

  
  
}
