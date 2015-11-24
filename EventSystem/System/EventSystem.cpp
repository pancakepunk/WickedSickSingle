#include "Precompiled.h"
#include "EventSystemPrecompiled.h"
#include "EventSystem.h"

#include "Utility/UtilityInterface.h"

#include "Event/EventCallback.h"
#include "Event/EventCallbackList.h"

#include "Event/Event.h"

namespace WickedSick
{
  EventSystem::EventSystem() : System(ST_EventSystem)
  {
    
  }

  EventSystem::~EventSystem()
  {

  }

  void EventSystem::Initialize()
  {
    //register engine wide events

    Register("Quit", QuitEvent);
  }

  bool EventSystem::Load()
  {
    return true;
  }

  bool EventSystem::Reload()
  {
    return true;
  }

  void EventSystem::Update(float dt)
  {
    FireEvents();
  }


  Event* EventSystem::QueueEvent(const std::string& eventName)
  {
    Event* newEvent = event_manager_.New(eventName);
    event_queue_.push_back(newEvent);
    return newEvent;
  }

  void EventSystem::Register(const std::string & eventName,
                                                EventCallback callback)
  {
    auto it = event_map_.find(eventName);
    if (it != event_map_.end())
    {
      auto& callbackList = (*it).val;
      callbackList.push_back(callback);
    }
    else
    {
      event_map_.insert(eventName, std::vector<EventCallback>(1, callback));
    }
  }

  void EventSystem::ReceiveMessage(Event * msg)
  {

  }

  void EventSystem::FireEvents()
  {
    for (auto& it : event_queue_)
    {
      auto& mapIt = event_map_.find(it->GetName());
      if(mapIt != event_map_.end())
      {
        auto& callbackList = (*mapIt).val;
        for (auto& cit : callbackList)
        {
          cit.Fire(it);
        }
      }
    }
    event_queue_.clear();
  }
  

}
