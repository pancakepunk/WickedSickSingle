#include "EventSystemPrecompiled.h"
#include "EventCallbackList.h"
#include "Core/CoreInterface.h"

namespace WickedSick
{

  void QuitEvent(Event *)
  {
    Engine::GetCore()->ShutDown();
  }
}
