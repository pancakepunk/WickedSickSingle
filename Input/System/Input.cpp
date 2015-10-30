#include "InputPrecompiled.h"
#include "Input.h"
#include "Handlers/InputBuffer.h"
#include "Handlers/InputHandler.h"


namespace WickedSick
{

  Input::Input() : System(ST_Input)
  {
    handler_ = new InputHandler();
  }

  Input::~Input()
  {

  }

  void Input::Initialize()
  {
  }
  
  bool Input::Load()
  {
    handler_->Load("nofile");
    return true;
  }
  
  bool Input::Reload()
  {
    return true;
  }
  
  void Input::Update(float dt)
  {
    handler_->Update(dt);
    
  }

  void Input::ReceiveMessage(Event * msg)
  {
  }

  void Input::UpdateInput(InputBuffer* buf)
  {
    handler_->UpdateInput(*buf);
  }

  InputHandler* Input::GetHandler()
  {
    return handler_;
  }


}
