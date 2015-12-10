#include "Precompiled.h"
#include "ParticleEffectEditor.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "Math/MathInterface.h"
#include "Graphics/GraphicsInterface.h"
#include "Meta/MetaInterface.h"
#include "TweakBarCallbacks.h"
#include "Editor/EditorInterface.h"
namespace WickedSick
{
  ParticleEditor::ParticleEditor() 
  : system_index_(0),
    emitter_index_(0),
    state_index_(0),
    system_window_(nullptr),
    emitter_window_(nullptr),
    state_window_(nullptr)
  {

  }

  ParticleEditor::~ParticleEditor()
  {
  }

  void ParticleEditor::Initialize()
  {
    system_window_ = TwNewBar("System Editor");
    TwAddVarCB(system_window_,
               "System Index", 
               TW_TYPE_INT32,
               TweakBarParticleSetSystem,
               TweakBarParticleGetSystem,
               nullptr,
               "");

    emitter_window_ = TwNewBar("Emitter Editor");
    TwAddVarCB(emitter_window_, 
               "Emitter Index", 
               TW_TYPE_INT32, 
               TweakBarParticleSetEmitter,
               TweakBarParticleGetEmitter,
               nullptr, 
               "");

    state_window_ = TwNewBar("State Editor");
    TwAddVarCB(state_window_, 
               "State Index", 
               TW_TYPE_INT32, 
               TweakBarParticleSetState,
               TweakBarParticleGetState,
               nullptr, 
               "");
  }

  void ParticleEditor::Update(float dt)
  {
    static bool init = true;
    if(init)
    {
      refresh_system_ui();
      init = false;
    }
    SelectSystem(system_index_);
    SelectEmitter(emitter_index_);
    SelectEmitterState(state_index_);
  }

  ParticleSystem * ParticleEditor::GetSelectedSystem()
  {
    Graphics* gSys = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    auto particleManager = gSys->GetParticleManager();
    return particleManager->GetSystem(system_index_);
  }

  ParticleEmitter * ParticleEditor::GetSelectedEmitter()
  {
    ParticleSystem* sys = GetSelectedSystem();
    if(sys)
    {
      return sys->GetEmitter(emitter_index_);
    }
    return nullptr;
  }

  ParticleDescription * ParticleEditor::GetSelectedEmitterState()
  {
    ParticleEmitter* emitter = GetSelectedEmitter();
    auto& states = emitter->GetStates();
    if(state_index_ < states.size())
    {
      return & states[state_index_];
    }
    return nullptr;
  }

  ParticleDescription * ParticleEditor::GetSelectedEmitterVariance()
  {
    ParticleEmitter* emitter = GetSelectedEmitter();
    auto& states = emitter->GetStateVariances();
    if(state_index_ < states.size())
    {
      return &states[state_index_];
    }
    return nullptr;
  }

  void ParticleEditor::SelectSystem(size_t systemIndex)
  {
    if(system_index_ == systemIndex)
    {
      return;
    }
    Graphics* gSys = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleComponent* comp = (ParticleComponent*) editor->GetCurrentSelected()->GetComponent(CT_ParticleComponent);
    
    auto particleManager = gSys->GetParticleManager();
    system_index_= systemIndex;
    system_index_ = clamp(system_index_, (size_t)0, comp->GetSystems().size() - 1);
    refresh_system_ui();
    
  }

  void ParticleEditor::SelectEmitter(size_t index)
  {
    if(emitter_index_ == index)
    {
      return;
    }
    ParticleSystem* sys = GetSelectedSystem();
    emitter_index_ = index;
    emitter_index_ = clamp(emitter_index_, (size_t)0, sys->GetEmitters().size() - 1);
    refresh_emitter_ui();
  }

  void ParticleEditor::SelectEmitterState(size_t index)
  {
    if(state_index_ == index)
    {
      return;
    }
    ParticleEmitter* emitter = GetSelectedEmitter();
    state_index_ = index;
    state_index_ = clamp(state_index_, (size_t) 0, emitter->GetStates().size() - 1);
    refresh_state_ui();
  }

  size_t ParticleEditor::GetEmitterStateIndex()
  {
    return state_index_;
  }

  size_t ParticleEditor::GetEmitterIndex()
  {
    return emitter_index_;
  }

  size_t ParticleEditor::GetSystemIndex()
  {
    return system_index_;
  }

  void ParticleEditor::Clear()
  {
    system_index_ = -1;
  }

  void ParticleEditor::refresh_system_ui()
  {
    per_system_info_.clear();
    TwRemoveAllVars(system_window_);
    TwAddVarCB(system_window_,
               "System Index",
               TW_TYPE_INT32,
               TweakBarParticleSetSystem,
               TweakBarParticleGetSystem,
               nullptr,
               "");
    TwAddButton(system_window_,
                "Remove System",
                TweakBarRemoveSystem,
                &system_index_,
                "");
    TwAddButton(system_window_,
                "Add Emitter",
                TweakBarAddEmitter,
                nullptr,
                "");
    Reflection::Metadata* systemMeta = FindType(ParticleSystem);
    ParticleCallbackInfo systemInfo;
    systemInfo.baseData = FindType(ParticleSystem);
    systemInfo.type = ParticleUiType::System;
    for(auto& it : systemMeta->GetMembers())
    {
      add_members(systemInfo, &it.second, system_window_);
    }


    

    refresh_emitter_ui();
    
  }

  void ParticleEditor::refresh_emitter_ui()
  {
    per_emitter_info_.clear();
    indices_for_tweakbar_.clear();
    TwRemoveAllVars(emitter_window_);
    TwAddVarCB(emitter_window_,
               "Emitter Index",
               TW_TYPE_INT32,
               TweakBarParticleSetEmitter,
               TweakBarParticleGetEmitter,
               nullptr,
               "");
    TwAddButton(emitter_window_,
                "Remove Emitter",
                TweakBarRemoveEmitter,
                &emitter_index_,
                "");
    TwAddButton(emitter_window_,
                "Add State",
                TweakBarAddState,
                nullptr,
                "");
    Reflection::Metadata* emitterMeta = FindType(ParticleEmitter);
    ParticleCallbackInfo emitterInfo;
    emitterInfo.baseData = emitterMeta;
    emitterInfo.type = ParticleUiType::Emitter;
    //first typical bullshit
    for(auto& it : emitterMeta->GetMembers())
    {
      add_members(emitterInfo, &it.second, emitter_window_);
    }
    //so here we have to add controls to toggle interpolation
    //either something interpolates or it doesn't no selective interpolation for certain states
    Reflection::Metadata* stateMeta = FindType(ParticleDescription);
    ParticleCallbackInfo stateInfo;
    stateInfo.baseData = FindType(ParticleDescription);
    stateInfo.type = ParticleUiType::State;
    
    
    per_emitter_info_.push_back(stateInfo);
    void* dataToSend = &per_emitter_info_.back();
    int index = 0;
    for(auto& it : stateMeta->GetMembers())
    {
      indices_for_tweakbar_.push_back(index);
      ++index;
      TwAddVarCB(emitter_window_,
                 it.second.GetName().c_str(),
                 TW_TYPE_STDSTRING,
                 TweakBarParticleSetRegistered,
                 TweakBarParticleGetRegistered,
                 &indices_for_tweakbar_.back(),
                 "group = 'registered vars'");
    }

    refresh_state_ui();
  }

  void ParticleEditor::refresh_state_ui()
  {
    per_state_info_.clear();
    TwRemoveAllVars(state_window_);
    TwAddVarCB(state_window_,
               "State Index",
               TW_TYPE_INT32,
               TweakBarParticleSetState,
               TweakBarParticleGetState,
               nullptr,
               "");
    TwAddButton(state_window_, 
                "Remove State", 
                TweakBarRemoveState, 
                &state_index_, 
                "");

    Reflection::Metadata* stateMeta = FindType(ParticleDescription);
    ParticleCallbackInfo stateInfo;
    stateInfo.baseData = FindType(ParticleDescription);
    stateInfo.type = ParticleUiType::State;
    //once for particle

    for(auto& it : stateMeta->GetMembers())
    {
      add_members(stateInfo, &it.second, state_window_, "state");
    }
    //once for variance
    stateInfo.type = ParticleUiType::Variance;
    for(auto& it : stateMeta->GetMembers())
    {
      add_members(stateInfo, &it.second, state_window_, "variance");
    }
  }

  void ParticleEditor::add_members(ParticleCallbackInfo& info, 
                                   Reflection::Member* base, 
                                   TwBar* bar,
                                   const std::string& group)
  {
    
    info.members.push_back(base);
    if(base->GetType() != FindType(Vector4))
    {
      for(auto& it : base->GetType()->GetMembers())
      {
        add_members(info, &it.second, bar, group);
      }
    }
    
    void* dataToSend = nullptr;
    switch(info.type)
    {
      case ParticleUiType::Emitter:
        per_emitter_info_.push_back(info);
        dataToSend = &per_emitter_info_.back();
        break;
      case ParticleUiType::System:
        per_system_info_.push_back(info);
        dataToSend = &per_system_info_.back();
        break;
      case ParticleUiType::State:
      case ParticleUiType::Variance:
        per_state_info_.push_back(info);
        dataToSend = &per_state_info_.back();
        break;
      default:
        __debugbreak();
    }

    std::string fullName = group + " ";
    Reflection::Metadata* type = base->GetType();
    for(auto& it : info.members)
    {
      fullName.append(it->GetName() + " ");
    }
    TwType varType = FindTwType(type);
    std::string tweakVars = " group='" + group + "'";
    if(varType == TW_TYPE_FLOAT || varType == TW_TYPE_COLOR4F)
    {
      tweakVars += " step=0.05 ";
    }
    if(varType == TW_TYPE_STDSTRING)
    {
      TwAddVarCB(bar,
                 fullName.c_str(),
                 FindTwType(type),
                 TweakBarParticleSetString,
                 TweakBarParticleGetString,
                 dataToSend,
                 tweakVars.c_str());
    }
    else
    {

      TwAddVarCB(bar,
                 fullName.c_str(),
                 FindTwType(type),
                 TweakBarParticleSet,
                 TweakBarParticleGet,
                 dataToSend,
                 tweakVars.c_str());
    }
    info.members.pop_back();
  }

}
