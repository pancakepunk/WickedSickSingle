#pragma once

#include "Particles/ParticleDescription.h"
#include "Utility/UtilityInterface.h"

namespace WickedSick
{
  class ParticleEmitter;

  class ParticleInterpolator
  {
  public:
    ParticleInterpolator();

    void SetTarget(const ParticleDescription& currentParticle,
                   const ParticleDescription& nextParticle,
                   size_t totalTargets,
                   size_t targetIndex);

    void Update(ParticleDescription& particle, ParticleEmitter* emitter);

    bool IsDone();

    float GetEndTime();
    size_t GetTargetIndex();
    static Random randomGen;

  private:
    
    bool done_;
    float end_time_;
    float prev_time_;
    size_t target_index_;
    ParticleDescription last_;
    ParticleDescription target_;
  };

}