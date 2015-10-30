#pragma once

#include "Core/CoreInterface.h"



namespace WickedSick
{
  struct InputBuffer;
  class InputHandler;
  class Input : public System
  {
  public:
    Input();
    ~Input();
    void Initialize();
    bool Load();
    bool Reload();
    void Update(float dt);
    void UpdateInput(InputBuffer* buf);
    void ReceiveMessage(Event * msg);

    InputHandler* GetHandler();
  private:
    InputHandler* handler_;

  };
}
