#pragma once


#include "anttweakbar/include/AntTweakBar.h"
#include "TweakBarCallbacks.h"
namespace Reflection
{
  class Member;
}



namespace WickedSick
{
  
  class ParticleSystem;
  class ParticleEmitter;
  class ParticleDescription;
  class ParticleEditor
  {
    public:
      ParticleEditor();
      ~ParticleEditor();
      void Initialize();
      void Update(float dt);

      ParticleSystem* GetSelectedSystem();
      ParticleEmitter* GetSelectedEmitter();
      ParticleDescription* GetSelectedEmitterState();
      ParticleDescription* GetSelectedEmitterVariance();
      void SelectSystem(size_t systemIndex);
      void SelectEmitter(size_t index);
      void SelectEmitterState(size_t index);

      size_t GetEmitterStateIndex();
      size_t GetEmitterIndex();
      size_t GetSystemIndex();

      void Clear();

    private:

      void refresh_system_ui();
      void refresh_emitter_ui();
      void refresh_state_ui();
      void add_members(ParticleCallbackInfo& info, 
                       Reflection::Member* base, 
                       TwBar* bar,
                       const std::string& prename = "");

      // these four are honestly pretty bad...
      std::list<ParticleCallbackInfo> per_system_info_;
      std::list<ParticleCallbackInfo> per_emitter_info_;
      std::list<int> indices_for_tweakbar_;
      std::list<ParticleCallbackInfo> per_state_info_;

      size_t system_index_;
      size_t emitter_index_;
      size_t state_index_;

      TwBar* system_window_;
      TwBar* emitter_window_;
      TwBar* state_window_;
  };
}