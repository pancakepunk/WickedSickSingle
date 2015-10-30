#pragma once
#include "Core/CoreInterface.h"
#include "Event/Event.h"


struct ID3D11RenderTargetView;
namespace WickedSick
{
  
  class Event;
  class EventCallback;
  class EventSystem : public System
  {
  public:
    EventSystem();
    ~EventSystem();
    void Initialize();
    bool Load();
    bool Reload();
    void Update(float dt);

    //point is invalidated by next frame, only for initialization
    Event* QueueEvent(const std::string& eventName);

    void Register( const std::string& eventName,
                                      EventCallback callback);

    void ReceiveMessage(Event * msg);



  private:

    void FireEvents();

    HashMap<std::string, std::vector<EventCallback>> event_map_;
    std::vector<Event*> event_queue_;

    MemoryManager<Event> event_manager_;

  };
}
