#include "Precompiled.h"
#include "CorePrecompiled.h"
#include "System.h"

namespace WickedSick
{
  System::System(SystemType type) : type_(type),
                                                core_(nullptr)
  {

  }

  System::~System()
  {

  }

  void System::SetCore(Engine* core)
  {
    core_ = core;
  }

  Engine* System::GetCore() const
  {
    return core_;
  }

  SystemType System::GetType()
  {
    return type_;
  }
}

