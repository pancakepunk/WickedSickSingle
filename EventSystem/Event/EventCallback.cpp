#include "EventSystemPrecompiled.h"
#include "EventCallback.h"

namespace WickedSick
{
  EventCallback::EventCallback( Callback fnPtr)
                                : callback_(fnPtr)
  {
  }

  EventCallback::EventCallback( const std::string& script)
                                : script_(script),
                                  callback_(nullptr)
  {
  }

  EventCallback::~EventCallback()
  {
  }

  void EventCallback::Fire(Event* myEvent)
  {
    if (callback_)
    {
      (*callback_)(myEvent);
    }
    //run script
  }
}
