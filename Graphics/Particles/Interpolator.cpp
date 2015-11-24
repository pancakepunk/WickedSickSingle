#include "Precompiled.h"
#include "Interpolator.h"


#include "Core/CoreInterface.h"

#include "ParticleEmitter.h"


namespace WickedSick
{
  Random ParticleInterpolator::randomGen;

  ParticleInterpolator::ParticleInterpolator()
  {
  }
 

  void ParticleInterpolator::SetTarget(const ParticleDescription& currentParticle,
                                       const ParticleDescription& nextParticle,
                                       size_t totalTargets,
                                       size_t targetIndex)
  {
    last_ = currentParticle;
    target_ = nextParticle;
    if(totalTargets == 1)
    {
      end_time_ = currentParticle.Lifetime;
      prev_time_ = 0.0f;
    }
    else
    {
      prev_time_ = float(targetIndex - 1) / float(totalTargets);
      end_time_ = float(targetIndex) / float(totalTargets);
      prev_time_ *= currentParticle.Lifetime;
      end_time_ *= currentParticle.Lifetime;
    }
    done_ = false;
    target_index_ = targetIndex;
  }


  #define SetParticleMember(memberSet, member, t) \
  if(memberSet.find(FindType(ParticleDescription)->GetMember(#member)) != memberSet.end()) \
  { \
    particle.member = last_.member + (target_.member - last_.member) * t; \
  }
  

  void ParticleInterpolator::Update(ParticleDescription& particle, ParticleEmitter* emitter)
  {
    float t = (particle.TimePassed - prev_time_) / end_time_;
    if(t >= 1.0f)
    {
      t = 1.0f;
      done_ = true;
    }
    SetParticleMember(emitter->members_to_interpolate_, Position, t);
    SetParticleMember(emitter->members_to_interpolate_, Velocity, t);
    SetParticleMember(emitter->members_to_interpolate_, Color, t);
    SetParticleMember(emitter->members_to_interpolate_, Scale, t);
    SetParticleMember(emitter->members_to_interpolate_, Rotation, t);
    
  }

  bool ParticleInterpolator::IsDone()
  {
    return done_;
  }

  float ParticleInterpolator::GetEndTime()
  {
    return end_time_;
  }

  size_t ParticleInterpolator::GetTargetIndex()
  {
    return target_index_;
  }



}
