#include "Precompiled.h"
#include "EditorPrecompiled.h"
#include "TweakBarCallbacks.h"
#include "ObjectFactory/ObjectFactoryInterface.h"
#include "System/Editor.h"
#include "Graphics/GraphicsInterface.h"
#include "Physics/PhysicsInterface.h"
#include "Meta/MetaInterface.h"
namespace WickedSick
{

  void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
  {
    // Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
    destinationClientString = sourceLibraryString;
  }

  void TW_CALL CopyStdStringToLibrary(std::string& destinationClientString, const std::string& sourceLibraryString)
  {
    // Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
    destinationClientString = sourceLibraryString;
  }

  void TW_CALL SetMyStdStringCB(const void *value, void * /*clientData*/)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    // Set: copy the value of s from AntTweakBar
    const std::string *srcPtr = static_cast<const std::string *>(value);
    editor->SetCurrentArchetype(*srcPtr);
  }
  void TW_CALL GetMyStdStringCB(void *value, void * /*clientData*/)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    // Get: copy the value of s to AntTweakBar
    std::string *destPtr = static_cast<std::string *>(value);
    TwCopyStdStringToLibrary(*destPtr, editor->GetCurrentArchetype()); // the use of TwCopyStdStringToLibrary is required here
  }


  char* findObject(Reflection::Metadata* type, GameObject* object)
  {
    if(type == FindType(Transform))
    {
      return (char*) object->GetComponent(CT_Transform);
    }
    else if(type == FindType(ModelComponent))
    {
      return (char*) object->GetComponent(CT_ModelComponent);
    }
    else if(type == FindType(LightComponent))
    {
      return (char*) object->GetComponent(CT_LightComponent);
    }
    else if(type == FindType(PhysicsComponent))
    {
      return (char*) object->GetComponent(CT_PhysicsComponent);
    }
    else if(type == FindType(DemoComponent))
    {
      return (char*) object->GetComponent(CT_DemoComponent);
    }
    else if(type == FindType(ReflectComponent))
    {
      return (char*) object->GetComponent(CT_ReflectComponent);
    }
    //else if(type == FindType(ParticleComponent))
    //{
    //  return (char*) object->GetComponent(CT_ParticleComponent);
    //}
    return nullptr;
  }

  void TW_CALL SetTwSelected(const void *value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    editor->Deselect();
    editor->Select(*(const int*)value);
  }

  void TW_CALL GetTwSelected(void *value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    *(int*)value = editor->GetSelectedId();
  }

  void TW_CALL TweakBarSet(const void *value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    CallbackInfo* info = (CallbackInfo*) clientData;
    GameObject* obj = editor->GetCurrentSelected();
    char* toSet = findObject(info->baseData, obj);
    for(auto& it : info->members)
    {
      toSet += it->GetOffset();
    }

    memcpy(toSet, value, info->members.back()->GetType()->GetSize());
    PhysicsComponent* physics = (PhysicsComponent*)obj->GetComponent(CT_PhysicsComponent);
    if(physics)
    {
      physics->Reset();
    }

    LightComponent* light = (LightComponent*) obj->GetComponent(CT_LightComponent);
    if(light)
    {
      light->Dirty();
    }
  }

  void TW_CALL TweakBarGet(void *value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    CallbackInfo* info = (CallbackInfo*) clientData;
    GameObject* obj = editor->GetCurrentSelected();
    char* toGet = findObject(info->baseData, obj);
    for(auto& it : info->members)
    {
      toGet += it->GetOffset();
    }

    memcpy(value, toGet, info->members.back()->GetType()->GetSize());
    //PhysicsComponent* physics = (PhysicsComponent*) obj->GetComponent(CT_PhysicsComponent);
    //if(physics)
    //{
    //  physics->Reset();
    //}
  }


  void TW_CALL TweakBarSetString(const void *value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    CallbackInfo* info = (CallbackInfo*) clientData;
    GameObject* obj = editor->GetCurrentSelected();
    char* toSet = findObject(info->baseData, obj);
    for(auto& it : info->members)
    {
      toSet += it->GetOffset();
    }
    *(std::string*)toSet = *(std::string*)value;
    //memcpy(toSet, value, info->members.back()->GetType()->GetSize());
    PhysicsComponent* physics = (PhysicsComponent*) obj->GetComponent(CT_PhysicsComponent);
    if(physics)
    {
      physics->Reset();
    }

    LightComponent* light = (LightComponent*) obj->GetComponent(CT_LightComponent);
    if(light)
    {
      light->Dirty();
    }
  }

  void TW_CALL TweakBarGetString(void *value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    CallbackInfo* info = (CallbackInfo*) clientData;
    GameObject* obj = editor->GetCurrentSelected();
    char* toGet = findObject(info->baseData, obj);
    for(auto& it : info->members)
    {
      toGet += it->GetOffset();
    }
    TwCopyStdStringToLibrary(*(std::string*) value, *(std::string*)toGet);
    //memcpy(value, toGet, info->members.back()->GetType()->GetSize());
    //PhysicsComponent* physics = (PhysicsComponent*) obj->GetComponent(CT_PhysicsComponent);
    //if(physics)
    //{
    //  physics->Reset();
    //}
  }









  void TW_CALL CopyCdStringToClient(char **destinationClientStringPtr, const char *sourceString)
  {
    if(*destinationClientStringPtr)
    {
      if(strlen(*destinationClientStringPtr) != strlen(sourceString))
      {
        delete[] *destinationClientStringPtr;
        *destinationClientStringPtr = new char[strlen(sourceString)];
      }
    }
    else
    {
      *destinationClientStringPtr = new char[strlen(sourceString)];
    }
    strcpy(*destinationClientStringPtr, sourceString);
  }

  TwType FindTwType(Reflection::Metadata * data)
  {
    if(!data->GetEnumConstants().empty())
    {
      return TW_TYPE_INT32;
    }
    else if(data == FindType(int))
    {
      return TW_TYPE_INT32;
    }
    else if(data == FindType(float))
    {
      return TW_TYPE_FLOAT;
    }
    else if(data == FindType(Vector4))
    {
      return TW_TYPE_COLOR4F;
    }
    else if(data == FindType(std::string))
    {
      return TW_TYPE_STDSTRING;
    }

    return TW_TYPE_UNDEF;
  }

  void TW_CALL ToggleParticleEditor(void * clientData)
  {
    bool& showParticleEditor = *(bool*)clientData;
    showParticleEditor = !showParticleEditor;
    //donesies lolololol
  }

  void TW_CALL NewObjectCallback(void * clientData)
  {
    std::string* name = (std::string*)clientData;
    ObjectFactory* fact = (ObjectFactory*) Engine::GetCore()->GetSystem(ST_ObjectFactory);
    GameObject* newObject = fact->CloneArchetype(*name);
    if(newObject)
    {
      Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
      editor->Deselect();
      
      Transform* tr = (Transform*)newObject->GetComponent(CT_Transform);
      tr->SetPosition(5, 5, 5);
      ModelComponent* mod = (ModelComponent*) newObject->GetComponent(CT_ModelComponent);
      if(mod)
      {
        mod->SetShader("pixelblinn");

        Material& mat = mod->GetMaterial();

        mat.ambientColor = Vector4(0.2f, 0.0f, 0.0f, 1.0f);
        mat.specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        mat.emissiveColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
        mat.diffuseColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

        mat.ambientConstant = 0.3f;
        mat.specularConstant = 1.0f;
        mat.emissiveConstant = 0.3f;
        mat.diffuseConstant = 0.5f;
        mat.shininess = 50.0f;
      }
     

      PhysicsComponent* bunnyPhysics = (PhysicsComponent*) newObject->GetComponent(CT_PhysicsComponent);
      if(bunnyPhysics)
      {
        RigidBody* bunnyBody = bunnyPhysics->GetRigidBody();
        bunnyBody->SetGravityScalar(0.0f);
        bunnyPhysics->SetPosition(tr->GetPosition());
      }
      editor->Select(newObject->GetID());

   
    }
  }

  void TW_CALL TweakBarAddSystem(void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleComponent* comp = (ParticleComponent*)editor->GetCurrentSelected()->GetComponent(CT_ParticleComponent);
    Graphics* graphics = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    ParticleManager* manager = graphics->GetParticleManager();
    ParticleEditor* pEdit = editor->GetParticleEditor();
    ParticleSystem* newSystem = manager->MakeParticleSystem();

    newSystem->Initialize();
    comp->AddParticleSystem(newSystem);
  }

  void TW_CALL TweakBarRemoveSystem(void * clientData)
  {
    int systemToRemove = *(int*) clientData;
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleComponent* comp = (ParticleComponent*) editor->GetCurrentSelected()->GetComponent(CT_ParticleComponent);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    comp->RemoveParticleSystem(systemToRemove);

    pEdit->SelectSystem(0);
  }

  void TW_CALL TweakBarAddEmitter(void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    Graphics* graphics = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    ParticleManager* manager = graphics->GetParticleManager();
    ParticleEditor* pEdit = editor->GetParticleEditor();
    ParticleEmitter* newEmitter = manager->MakeParticleEmitter();
    pEdit->GetSelectedSystem()->AddEmitter(newEmitter);
  }

  void TW_CALL TweakBarRemoveEmitter(void * clientData)
  {
    int emitterToRemove = *(int*) clientData;
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    ParticleSystem* sys = pEdit->GetSelectedSystem();
    if(sys->GetEmitters().size())
    {
      sys->RemoveEmitter(emitterToRemove);
      pEdit->SelectEmitter(0);
    }
  }

  void TW_CALL TweakBarAddState(void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    ParticleEmitter* emitter = pEdit->GetSelectedEmitter();
    emitter->AddParticleState();
  }

  void TW_CALL TweakBarRemoveState(void * clientData)
  {
    int stateToRemove = *(int*)clientData;
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    ParticleEmitter* emitter = pEdit->GetSelectedEmitter();
    if(emitter->GetStates().size())
    {
      emitter->RemoveParticleState(stateToRemove);
      pEdit->SelectEmitterState(0);
    }
  }

  void TW_CALL TweakBarParticleSetRegistered(const void *value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    ParticleEmitter* emitter = pEdit->GetSelectedEmitter();
    auto& attributes = emitter->GetAttributes();
    Reflection::Metadata* particleMeta = FindType(ParticleDescription);
    auto& members = particleMeta->GetMembers();

    std::string status = *(const std::string*)value;
    int iterator = 0;
    int index = *(int*) clientData;
    for(auto& it : members)
    {
      if(iterator != index)
      {
        ++iterator;
      }
      else//found the member it's looking for
      {
        if(status == "Registered")
        {
          emitter->RegisterAttribute(&it.second);
        }
        else if(status == "Unregistered")
        {
          emitter->UnregisterAttribute(&it.second);
        }
        else
        {
          //eh u fucked up
        }

        break;
      }
    }

  }

  void TW_CALL TweakBarParticleGetRegistered(void *value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    ParticleEmitter* emitter = pEdit->GetSelectedEmitter();
    auto& attributes = emitter->GetAttributes();
    Reflection::Metadata* particleMeta = FindType(ParticleDescription);
    auto& members = particleMeta->GetMembers();

    std::string status = "Unregistered";
    int iterator = 0;
    int index = *(int*)clientData;
    for(auto& it : members)
    {
      if(iterator != index)
      {
        ++iterator;
      }
      else//found the member it's looking for
      {
        for(auto& ait : attributes)//if it's in the attributes it's registered
        {
          if(ait == &it.second)
          {
            status = "Registered";
            break;
          }
        }
        break;
      }
    }

    TwCopyStdStringToLibrary(*(std::string*) value, status);

  }

  void TW_CALL TweakBarParticleSetState(const void * value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    pEdit->SelectEmitterState(*(int*)value);
  }

  void TW_CALL TweakBarParticleGetState(void * value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    *(int*)value = pEdit->GetEmitterStateIndex();
  }

  void TW_CALL TweakBarParticleSetEmitter(const void * value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    pEdit->SelectEmitter(*(int*) value);
  }

  void TW_CALL TweakBarParticleGetEmitter(void * value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    *(int*) value = pEdit->GetEmitterIndex();
  }

  void TW_CALL TweakBarParticleSetSystem(const void * value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    pEdit->SelectSystem(*(int*) value);
  }

  void TW_CALL TweakBarParticleGetSystem(void * value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleEditor* pEdit = editor->GetParticleEditor();
    *(int*) value = pEdit->GetSystemIndex();
  }



  //particles callbacks

  void TW_CALL TweakBarParticleSet(const void *value, void * clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleCallbackInfo* info = (ParticleCallbackInfo*) clientData;
    ParticleEditor* pEdit = editor->GetParticleEditor();
    char* toSet = nullptr;
    switch(info->type)
    {
      case ParticleUiType::Emitter:
        toSet = (char*)pEdit->GetSelectedEmitter();
        break;
      case ParticleUiType::System:
        toSet = (char*) pEdit->GetSelectedSystem();
        break;
      case ParticleUiType::State:
        toSet = (char*) pEdit->GetSelectedEmitterState();
        break;
      case ParticleUiType::Variance:
        toSet = (char*) pEdit->GetSelectedEmitterVariance();
        break;
    }
    for(auto& it : info->members)
    {
      toSet += it->GetOffset();
    }

    memcpy(toSet, value, info->members.back()->GetType()->GetSize());

  }

  void TW_CALL TweakBarParticleGet(void* value, void* clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleCallbackInfo* info = (ParticleCallbackInfo*) clientData;
    ParticleEditor* pEdit = editor->GetParticleEditor();
    char* toGet = nullptr;
    switch(info->type)
    {
      case ParticleUiType::Emitter:
        toGet = (char*) pEdit->GetSelectedEmitter();
        break;
      case ParticleUiType::System:
        toGet = (char*) pEdit->GetSelectedSystem();
        break;
      case ParticleUiType::State:
        toGet = (char*) pEdit->GetSelectedEmitterState();
        break;
      case ParticleUiType::Variance:
        toGet = (char*) pEdit->GetSelectedEmitterVariance();
        break;
    }
    for(auto& it : info->members)
    {
      toGet += it->GetOffset();
    }

    memcpy(value, toGet, info->members.back()->GetType()->GetSize());
  }


  void TW_CALL TweakBarParticleSetString(const void* value, void* clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleCallbackInfo* info = (ParticleCallbackInfo*) clientData;
    ParticleEditor* pEdit = editor->GetParticleEditor();
    char* toSet = nullptr;
    switch(info->type)
    {
      case ParticleUiType::Emitter:
        toSet = (char*) pEdit->GetSelectedEmitter();
        break;
      case ParticleUiType::System:
        toSet = (char*) pEdit->GetSelectedSystem();
        break;
      case ParticleUiType::State:
        toSet = (char*) pEdit->GetSelectedEmitterState();
        break;
      case ParticleUiType::Variance:
        toSet = (char*) pEdit->GetSelectedEmitterVariance();
        break;
    }
    for(auto& it : info->members)
    {
      toSet += it->GetOffset();
    }
    *(std::string*)toSet = *(std::string*)value;

  }

  void TW_CALL TweakBarParticleGetString(void* value, void* clientData)
  {
    Editor* editor = (Editor*) Engine::GetCore()->GetSystem(ST_Editor);
    ParticleCallbackInfo* info = (ParticleCallbackInfo*) clientData;
    ParticleEditor* pEdit = editor->GetParticleEditor();
    char* toGet = nullptr;
    switch(info->type)
    {
      case ParticleUiType::Emitter:
        toGet = (char*) pEdit->GetSelectedEmitter();
        break;
      case ParticleUiType::System:
        toGet = (char*) pEdit->GetSelectedSystem();
        break;
      case ParticleUiType::State:
        toGet = (char*) pEdit->GetSelectedEmitterState();
        break;
      case ParticleUiType::Variance:
        toGet = (char*) pEdit->GetSelectedEmitterVariance();
        break;
    }
    for(auto& it : info->members)
    {
      toGet += it->GetOffset();
    }
    TwCopyStdStringToLibrary(*(std::string*) value, *(std::string*)toGet);
    //memcpy(value, toGet, info->members.back()->GetType()->GetSize());
    //PhysicsComponent* physics = (PhysicsComponent*) obj->GetComponent(CT_PhysicsComponent);
    //if(physics)
    //{
    //  physics->Reset();
    //}
  }


}
