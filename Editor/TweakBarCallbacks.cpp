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
    if(data == FindType(int))
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
    else if(data == FindType(LightType::Enum))
    {
      return TW_TYPE_INT32;
    }
    else if(data == FindType(DrawType::Enum))
    {
      return TW_TYPE_INT32;
    }

    return TW_TYPE_UNDEF;
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
}
