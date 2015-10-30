#include "GraphicsPrecompiled.h"
#include "Graphics/GraphicsInterface.h"
#include "ModelInstance.h"

namespace WickedSick
{
  ModelInstance::ModelInstance()  : parent(nullptr)
  {

  }

  void ModelInstance::Destroy()
  {
   //auto& toRemove = std::find(parent->GetInstances().begin(), 
   //                           parent->GetInstances().end(), 
   //                           this);
   //parent->GetInstances().erase(toRemove);
   //Model::instanceFactory.Destroy(this);
  }


}
