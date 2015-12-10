#pragma once
#include "anttweakbar/include/AntTweakBar.h"

#include "Meta/MetaInterface.h"


namespace WickedSick
{

  struct CallbackInfo
  {
    Reflection::Metadata* baseData;
    std::vector<Reflection::Member*> members;
  };
  namespace ParticleUiType
  {
    enum Enum
    {
      Emitter,
      System,
      State,
      Variance,
      Count
    };
  }
  struct ParticleCallbackInfo
  {
    ParticleUiType::Enum type;
    Reflection::Metadata* baseData;
    std::vector<Reflection::Member*> members;
  };

  TwType FindTwType(Reflection::Metadata* data);
  
  void TW_CALL ToggleParticleEditor(void* clientData);

  void TW_CALL NewObjectCallback(void* clientData);
  void TW_CALL CopyCdStringToClient(char **destinationClientStringPtr, const char *sourceString);
  void TW_CALL GetMyStdStringCB(void *value, void * /*clientData*/);
  void TW_CALL SetMyStdStringCB(const void *value, void * /*clientData*/);

  void TW_CALL TweakBarSet(const void *value, void * clientData);
  void TW_CALL TweakBarGet(void *value, void * clientData);
  void TW_CALL TweakBarSetString(const void *value, void * clientData);
  void TW_CALL TweakBarGetString(void *value, void * clientData);

  void TW_CALL SetTwSelected(const void *value, void * clientData);

  void TW_CALL GetTwSelected(void *value, void * clientData);

  void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString);
  
  void TW_CALL TweakBarAddState(void * clientData);
  void TW_CALL TweakBarRemoveState(void * clientData);
  void TW_CALL TweakBarAddEmitter(void * clientData);
  void TW_CALL TweakBarRemoveEmitter(void * clientData);
  void TW_CALL TweakBarAddSystem(void * clientData);
  void TW_CALL TweakBarRemoveSystem(void * clientData);

  void TW_CALL TweakBarParticleSetRegistered(const void *value, void * clientData);
  void TW_CALL TweakBarParticleGetRegistered(void *value, void * clientData);


  void TW_CALL TweakBarParticleSetState(const void* value, void * clientData);
  void TW_CALL TweakBarParticleGetState(void *value, void * clientData);
  void TW_CALL TweakBarParticleSetEmitter(const void* value, void * clientData);
  void TW_CALL TweakBarParticleGetEmitter(void *value, void * clientData);
  void TW_CALL TweakBarParticleSetSystem(const void* value, void * clientData);
  void TW_CALL TweakBarParticleGetSystem(void *value, void * clientData);

  void TW_CALL TweakBarParticleSet(const void* value, void * clientData);
  void TW_CALL TweakBarParticleGet(void *value, void * clientData);
  void TW_CALL TweakBarParticleSetString(const void *value, void * clientData);
  void TW_CALL TweakBarParticleGetString(void *value, void * clientData);
  //void TW_CALL TweakBarSystemSet(const void *value, void * clientData);
  //void TW_CALL TweakBarSystemGet(void *value, void * clientData);
  //void TW_CALL TweakBarSystemSetString(const void *value, void * clientData);
  //void TW_CALL TweakBarSystemGetString(void *value, void * clientData);
}

