#include "EventSystemPrecompiled.h"
#include "Event.h"

#include "System/EventSystem.h"

namespace WickedSick
{
  EventSystem* Event::event_system_;

  int Event::id_count_ = 0;

  Event::Event(const std::string& name) 
                                  : event_name_(name),
                                    event_ID_(id_count_++)
  {
    
  }


  Event::~Event()
  {
    
  }

  std::string Event::GetName()
  {
    return event_name_;
  }

  
}
