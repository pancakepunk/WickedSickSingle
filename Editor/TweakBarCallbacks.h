#pragma once
#include "anttweakbar/include/AntTweakBar.h"
#include "meta/MetaInterface.h"
namespace WickedSick
{
  TwType FindTwType(Reflection::Metadata* data);
  

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
}