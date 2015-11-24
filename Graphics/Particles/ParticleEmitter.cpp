#include "Precompiled.h"
#include "ParticleEmitter.h"
#include "Graphics/Model/Model.h"

#include "Core/CoreInterface.h"
#include "Graphics/GraphicsInterface.h"


#include "ParticleSystem.h"

namespace WickedSick
{

  template<typename T>
  T RandVariance(const T& in, const T& variance)
  {
    T toReturn = in;
    float scalar = std::normal_distribution(-1.0f, 1.0f);
    in += scalar * variance;
  }

  ParticleEmitter::ParticleEmitter(size_t particleCount,
                                   const EmitterDescription& description,
                                   ParticleSystem* base)
  : burst_timer_(0.0f),
    emit_timer_(0.1f),
    description_(description),
    base_(base)
  {
    emit_timer_ = description_.emitLength;
    particles_.resize(particleCount);
    interpolators_.resize(particleCount);
    for(size_t i = 0; i < particleCount; ++i)
    {
      dead_.insert(i);
    }
    
  }

  void ParticleEmitter::Update(float dt)
  {
    burst_timer_ += dt;
    
    if(description_.lifetime > 0.0f)
    {
      description_.lifetime -= dt;
      //unlikely, but just in case....
      if(description_.lifetime == 0.0f)
      {
        description_.lifetime -= 0.01f;
      }
    }
    
    if(burst_timer_ > description_.frequency)
    {
      burst_timer_ = 0.0f;
      emit_timer_ = 0.0f;
    }
    if(emit_timer_ < description_.emitLength)
    {
      emit_timer_ += dt;
      //emit amountToSpawn
      Emit(description_.amountToSpawn);
    }

    int index;
    for(auto it = live_.begin(); it != live_.end(); )
    {
      index = *it;
      
      ParticleDescription& particle = particles_[index];
      ParticleInterpolator* interpolator = &interpolators_[index];
      if(particle.TimePassed <= particle.Lifetime)
      {
        interpolator->Update(particle, this);
       
        

        particle.PrevPos = particle.Position;
        particle.Position += particle.Velocity * dt;

        //handle zero case
        if(particle.Lifetime)
        {
          particle.TimePassed += dt;
        }
        
        if(interpolator->IsDone())
        {
          ParticleDescription newParticle;
          size_t numStates = states_.size();
          size_t newIndex = interpolator->GetTargetIndex() + 1;
          init_particle(newParticle,
                        std::min(numStates - 1, newIndex));
          interpolator->SetTarget(particle,
                                  newParticle,
                                  numStates,
                                  newIndex);
        }
        
        ++it;
      }
      else
      {
        it = live_.erase(it);
        dead_.insert(index);
      }
    }
  }



  EmitterDescription & ParticleEmitter::GetEmitterDescription()
  {
    return description_;
  }

  bool ParticleEmitter::IsDead()
  {
    return description_.lifetime < 0.0f;
  }

  void ParticleEmitter::Kill()
  {
    description_.lifetime = -1.0f;
  }

  void ParticleEmitter::Emit(size_t count)
  {
    size_t deadBegin;
    while(count && !dead_.empty())
    {
      deadBegin = *dead_.begin();
      live_.insert(deadBegin);
      init_particle(particles_[deadBegin], 0);
      if(states_.size() > 1)
      {
        ParticleDescription newParticle;
        init_particle(newParticle, 1);
        interpolators_[deadBegin].SetTarget(particles_[deadBegin], newParticle, states_.size(), 1);
      }
      else
      {
        interpolators_[deadBegin].SetTarget(particles_[deadBegin], particles_[deadBegin], states_.size(), 0);
      }
      dead_.erase(deadBegin);
      --count;
    }
  }

  void ParticleEmitter::AddParticleState(const ParticleDescription & particle, const ParticleDescription & variance)
  {
    states_.push_back(particle);
    variance_.push_back(variance);
  }

  void ParticleEmitter::ClearParticleStates()
  {
    states_.clear();
    variance_.clear();
  }

  void ParticleEmitter::RegisterAttribute(const std::string & memberName)
  {
    auto particleDescMeta = FindType(ParticleDescription);
    auto member = particleDescMeta->GetMember(memberName);
    members_to_interpolate_.insert(member);
  }

  ParticleSystem * ParticleEmitter::GetSystem()
  {
    return base_;
  }

  void ParticleEmitter::Render()
  {
    Graphics* gSys = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    gSys->GetModel(description_.sourceModel)->Render();
  }

  std::set<size_t>& ParticleEmitter::GetAlive()
  {
    return live_;
  }

  std::vector<ParticleDescription>& ParticleEmitter::GetParticles()
  {
    return particles_;
  }

  void ParticleEmitter::init_particle(ParticleDescription & particle, size_t index)
  {
    ParticleInterpolator::randomGen.Gen();
    if(states_.empty())
    {
      __debugbreak();
    }
    particle.Color.x = states_[index].Color.x + ParticleInterpolator::randomGen.Gen() * (variance_[index].Color.x);
    particle.Color.y = states_[index].Color.y + ParticleInterpolator::randomGen.Gen() * (variance_[index].Color.y);
    particle.Color.z = states_[index].Color.z + ParticleInterpolator::randomGen.Gen() * (variance_[index].Color.z);
    particle.Color.w = states_[index].Color.w + ParticleInterpolator::randomGen.Gen() * (variance_[index].Color.w);

    particle.Position.x = states_[index].Position.x + ParticleInterpolator::randomGen.Gen() * (variance_[index].Position.x);
    particle.Position.y = states_[index].Position.y + ParticleInterpolator::randomGen.Gen() * (variance_[index].Position.y);
    particle.Position.z = states_[index].Position.z + ParticleInterpolator::randomGen.Gen() * (variance_[index].Position.z);
    particle.Lifetime   = states_[index].Lifetime + ParticleInterpolator::randomGen.Gen() * (variance_[index].Lifetime);
    
    particle.Rotation   = states_[index].Rotation + ParticleInterpolator::randomGen.Gen() * (variance_[index].Rotation);
    particle.Scale      = states_[index].Scale    + ParticleInterpolator::randomGen.Gen() * (variance_[index].Scale);

    particle.Velocity.x = states_[index].Velocity.x + ParticleInterpolator::randomGen.Gen() * (variance_[index].Velocity.x);
    particle.Velocity.y = states_[index].Velocity.y + ParticleInterpolator::randomGen.Gen() * (variance_[index].Velocity.y);
    particle.Velocity.z = states_[index].Velocity.z + ParticleInterpolator::randomGen.Gen() * (variance_[index].Velocity.z);

    particle.PrevPos    = particle.Position;
    particle.TimePassed = 0.0f;
    
  }
}
