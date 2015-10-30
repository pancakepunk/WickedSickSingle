#include "GraphicsPrecompiled.h"
#include "Shader/ParamTypeInfo.h"
#include "Debugging/DebugInterface.h"

namespace WickedSick
{
  ParamPasser::ParamPasser(const std::string& buffer,
                           const std::string& name,
                           const void* data,
                           int size)
  : Buffer(buffer),
    Name(name),
    Size(size)
  {
    WSAssert(Size <= 900, "Shader Param Too Big!");
    memcpy(Data, data, Size);
  }

  ParamPasser::ParamPasser(const ParamPasser & toCopy) 
  : Buffer(toCopy.Buffer),
    Name(toCopy.Name),
    Size(toCopy.Size)
  {
    WSAssert(Size <= 900, "Shader Param Too Big!");
    memcpy(Data, toCopy.Data, Size);
  }

  ParamPasser::ParamPasser(ParamPasser && moveFrom)
  : Buffer(std::move(moveFrom.Buffer)),
    Name(std::move(moveFrom.Name)),
    Size(moveFrom.Size)
  {
    memcpy(Data, moveFrom.Data, Size);
  }

  void ParamPasser::Release()
  {
  }

  ParamPasser::~ParamPasser()
  {
    Release();
  }
}

