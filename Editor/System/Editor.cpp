#include "Precompiled.h"

#include "EditorPrecompiled.h"
#include "Editor.h"
#include "Core/CoreInterface.h"
#include "ObjectFactory/ObjectFactoryInterface.h"
#include "meta/MetaInterface.h"
#include "Graphics/GraphicsInterface.h"
#include "Physics/PhysicsInterface.h"
#include "TweakBarCallbacks.h"


namespace WickedSick
{
  Editor::Editor() 
  : System(ST_Editor),
    show_particle_editor_(false),
    particle_editor_(nullptr),
    selected_id_(0),
    archetype_index_(0),
    archetype_enum_((TwType)0),
    object_ui_(nullptr),
    scene_ui_(nullptr)
  {
    particle_editor_ = new ParticleEditor();
  }

  Editor::~Editor()
  {
    delete particle_editor_;
  }

  bool Editor::Load()
  {

    return true;
  }

  bool Editor::Reload()
  {
    return false;
  }

  void Editor::Initialize()
  {
    particle_editor_->Initialize();
    
    //create UI
    //archetype_to_clone_ = "box";
    //char* dumbstring = new char[sizeof("box")];
    //
    //scene_ui_ = TwNewBar("Scene Editor");
    ////TwCopyCDStringToClientFunc(CopyCdStringToClient);
    ////TwCopyStdStringToClientFunc(CopyStdStringToClient);
    //TwAddVarRW(scene_ui_, "Archetype", TW_TYPE_CDSTRING, &dumbstring, "");
    //auto err =TwGetLastError();
    //TwAddButton(scene_ui_, "Create New Object", &NewObjectCallback, &archetype_to_clone_, "");
    //err = TwGetLastError();
    //TwAddVarRW(scene_ui_, "Object ID", TW_TYPE_INT32, &selected_id_, " min=0 max=500 ");
    //err = TwGetLastError();
    
    //object_ui_ = TwNewBar("Object Editor");
    
  }

  void Editor::Update(float dt)
  {
    ObjectFactory* fact = (ObjectFactory*) Engine::GetCore()->GetSystem(ST_ObjectFactory);
    Graphics* gSys = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    static bool once = true;
    if(once)
    {
      auto archetypes = fact->GetArchetypes();
      for(auto& it : archetypes)
      {
        char* tweakEnum = new char[it.first.size() + 1];
        tweakEnum[it.first.size()] = '\0';
        memcpy(tweakEnum, it.first.c_str(), it.first.size());
        archetypeEnums.push_back({(int) archetypeEnums.size(), tweakEnum});
      }

      scene_ui_ = TwNewBar("Scene Editor");
      //TwAddButton(scene_ui_, "Create New Object", &NewObjectCallback, &archetype_to_clone_, "");
      TwAddVarCB(scene_ui_, "Object ID", TW_TYPE_INT32, SetTwSelected, GetTwSelected, nullptr, " min=0 max=500 ");
      TwAddButton(scene_ui_, "Particle System Editor", &ToggleParticleEditor, &show_particle_editor_, "");


      // Defining season enum type
      archetype_enum_ = TwDefineEnum("ArchetypeName", &archetypeEnums[0], archetypeEnums.size());
      // Adding season to bar
      archetype_index_ = 0;
      //TwAddVarRW(scene_ui_, "Archetype", archetype_enum_, &archetype_index_, NULL);
      auto& sceneConstants = gSys->GetSceneConstants();
      TwAddVarRW(scene_ui_, "AmbientColor", TW_TYPE_COLOR4F, &sceneConstants.ambientColor, NULL);
      TwAddVarRW(scene_ui_, "AmbientIntensity", TW_TYPE_FLOAT, &sceneConstants.ambientIntensity, NULL);
      TwAddVarRW(scene_ui_, "FogColor", TW_TYPE_COLOR4F, &sceneConstants.fogColor, NULL);
      TwAddVarRW(scene_ui_, "FogFar", TW_TYPE_FLOAT, &sceneConstants.fogFar, NULL);
      TwAddVarRW(scene_ui_, "FogNear", TW_TYPE_FLOAT, &sceneConstants.fogNear, NULL);
      TwAddVarRW(scene_ui_, "Rotate", TW_TYPE_BOOLCPP, &sceneConstants.rotate, NULL);
      

      object_ui_ = TwNewBar("Object Editor");
      Select(selected_id_);
      UpdateSelected();
      TwCopyStdStringToClientFunc(CopyStdStringToClient);
      once = false;
    }
    if(show_particle_editor_)
    {
      particle_editor_->Update(dt);
    }

    archetype_to_clone_ = archetypeEnums[archetype_index_].Label;
  }

  void Editor::ReceiveMessage(Event*)
  {
  }

  GameObject * Editor::GetCurrentSelected()
  {
    ObjectFactory* fact = (ObjectFactory*) Engine::GetCore()->GetSystem(ST_ObjectFactory);
    return fact->GetObject(selected_id_);
  }

  std::string Editor::GetCurrentArchetype()
  {
    return archetype_to_clone_;
  }

  void Editor::SetCurrentArchetype(const std::string & archetype)
  {
    archetype_to_clone_ = archetype;
  }

  void Editor::add_members(CallbackInfo& cbInfo, Reflection::Member* base)
  {
    cbInfo.members.push_back(base);
    for(auto& it : base->GetType()->GetMembers())
    {
      if(cbInfo.members.back()->GetType() != FindType(Vector4))
      {
        add_members(cbInfo, &it.second);
      }
      
    }
    per_object_info_.push_back(CallbackInfo(cbInfo));
    std::string fullName;
    for(size_t i = 0; i < cbInfo.members.size(); ++i)
    {
      fullName.append(cbInfo.members[i]->GetName() + " ");
    }
    TwType varType = FindTwType(cbInfo.members.back()->GetType());
    std::string tweakVars = " group='" + cbInfo.baseData->GetName() + "'";
    if(varType == TW_TYPE_FLOAT || varType == TW_TYPE_COLOR4F)
    {
      tweakVars += " step=0.05 ";
    }
    if(varType == TW_TYPE_STDSTRING)
    {
      TwAddVarCB(object_ui_,
                 fullName.c_str(),
                 FindTwType(cbInfo.members.back()->GetType()),
                 TweakBarSetString,
                 TweakBarGetString,
                 &per_object_info_.back(),
                 tweakVars.c_str());
    }
    else
    {

      TwAddVarCB(object_ui_,
                 fullName.c_str(),
                 FindTwType(cbInfo.members.back()->GetType()),
                 TweakBarSet,
                 TweakBarGet,
                 &per_object_info_.back(),
                 tweakVars.c_str());
    }
    
    cbInfo.members.pop_back();
    
  }


  void Editor::UpdateSelected()
  {
    ObjectFactory* fact = (ObjectFactory*) Engine::GetCore()->GetSystem(ST_ObjectFactory);
    GameObject* newObject = fact->GetObject(selected_id_);
    TwRemoveAllVars(object_ui_);

    per_object_info_.clear();
    //per_object_info_.reserve(200);
    
    CallbackInfo tempInfo;
    Reflection::Metadata* meta = nullptr;
    if(newObject)
    {
      for(auto& it : newObject->GetComponents())
      {
        if(!it)
        {
          continue;
        }
        switch(it->GetType())
        {
          case CT_LightComponent:
          {
            meta = Reflection::MetaSingleton<LightComponent>::Get();
            break;
          }
          case CT_PhysicsComponent:
            continue;
          case CT_Transform:
          {
            meta = Reflection::MetaSingleton<Transform>::Get();
            break;
          }
          case CT_ModelComponent:
          {
            meta = Reflection::MetaSingleton<ModelComponent>::Get();
            break;
          }
          case CT_DemoComponent:
          {
            meta = Reflection::MetaSingleton<DemoComponent>::Get();
            break;
          }

          case CT_ReflectComponent:
          {
            meta = Reflection::MetaSingleton<ReflectComponent>::Get();
            break;
          }
          case CT_ParticleComponent:
          {
            TwAddButton(object_ui_,
                        "Add System",
                        TweakBarAddSystem,
                        nullptr,
                        "group='Particle Component'");
            break;
          }
          default:
            continue;
        }
        if(meta)
        {
          tempInfo.baseData = meta;
          for(auto& it : meta->GetMembers())
          {
            add_members(tempInfo, &it.second);
          }
        }
        
      }

 

    }
  }

  void Editor::Select(GameObject * target)
  {
    selected_.insert(target->GetID());
    selected_id_ = target->GetID();
    UpdateSelected();
  }

  void Editor::Select(int id)
  {
    selected_.insert(id);
    selected_id_ = id;
    UpdateSelected();
  }

  void Editor::Deselect(GameObject * target)
  {
    selected_.erase(target->GetID());
    selected_id_ = -1;
    UpdateSelected();
  }

  void Editor::Deselect(int id)
  {
    selected_.erase(id);
    selected_id_ = -1;
    UpdateSelected();
  }

  void Editor::Deselect()
  {
    selected_.clear();
    selected_id_ = -1;
    UpdateSelected();
  }

  CallbackInfo & Editor::GetCBInfo()
  {
    return cb_info_;
  }

  int Editor::GetSelectedId()
  {
    return selected_id_;
  }

  ParticleEditor * Editor::GetParticleEditor()
  {
    return particle_editor_;
  }

}



