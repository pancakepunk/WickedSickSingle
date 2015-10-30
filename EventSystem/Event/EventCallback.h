#pragma once
#include <string>

namespace WickedSick
{
  class Event;
  class EventCallback
  {
  public:
    typedef void(*Callback)(Event*);
    EventCallback(const std::string& script);
    EventCallback(Callback fnPtr);
    ~EventCallback();
    void Fire(Event* myEvent);
  private:
    std::string script_;
    Callback callback_;
  };
}
