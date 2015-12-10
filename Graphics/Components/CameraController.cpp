#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "CameraController.h"
#include "CameraComponent.h"
#include "Input/InputInterface.h"
#include "Graphics/GraphicsInterface.h"

namespace WickedSick
{

  CameraController::CameraController() : Component(WickedSick::CT_CameraController)
  {
  }

  CameraController::~CameraController()
  {
  }

  void CameraController::Initialize()
  {
    Input* input = (Input*)Engine::GetCore()->GetSystem(ST_Input);
    InputHandler* handler = input->GetHandler();
    
  }

  void CameraController::Update(float dt)
  {
    Input* input = (Input*)Engine::GetCore()->GetSystem(ST_Input);
    InputHandler* handler = input->GetHandler();
    bool dirty = false;
    CameraComponent* cameraComp = (CameraComponent*) GetSibling(CT_CameraComponent);
    Transform* tr = (Transform*) GetSibling(CT_Transform);
    //cameraComp->SetRotation(tr->GetRotation());
    if (handler->Check("Rotate"))
    {
      if(last_mouse_pos_ == Vector2i(-1, -1))
      {
        last_mouse_pos_ = handler->GetMousePos();
      }
      else if(handler->GetMousePos() != last_mouse_pos_)
      {
        Vector2i curPos = handler->GetMousePos();
        Vector2 amountToRotate = (curPos - last_mouse_pos_).to_f();

        rotation_.x -= amountToRotate.y;
        rotation_.y -= amountToRotate.x;

        cameraComp->SetRotation(rotation_);
        last_mouse_pos_ = Vector2i(-1, -1);
        dirty = true;
      }
    }
    else if(handler->Check("Move"))
    {
      if (last_mouse_pos_ == Vector2i(-1, -1))
      {
        last_mouse_pos_ = handler->GetMousePos();
      }
      else if (handler->GetMousePos() != last_mouse_pos_)
      {
        Vector2i curPos = handler->GetMousePos();
        Vector2i amountToMove = (curPos - last_mouse_pos_);
        
        //Transform* tr = (Transform*)GetSibling(CT_Transform);

        

        //Vector3 pos = tr->GetPosition();
        //amountToMove *= 5.0f;

        //tr->SetPosition(pos);
        cameraComp->Move(Vector3i(amountToMove, 0));

        last_mouse_pos_ = Vector2i(-1, -1);
        dirty = true;
      }
    }
    else if (handler->Check("ZoomIn"))
    {


      //tr->SetPosition(tr->GetPosition() += view );
      cameraComp->Move(Vector3i(0, 0, 1));
      dirty = true;
    }
    else if (handler->Check("ZoomOut"))
    {
      cameraComp->Move(Vector3i(0, 0, -1));
      dirty = true;
    }
    else if (handler->GetScrollPos())
    {

      cameraComp->Move(Vector3i(0, 0, handler->GetScrollPos()));
      dirty = true;
    }
    else
    {
      last_mouse_pos_ = Vector2i(-1, -1);
    }

    Graphics* gSys = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    if(dirty)
    {
      gSys->GetCamera()->Dirty();
    }
    
  }

  void CameraController::Clone(Component * source)
  {
    CameraController* camCont = (CameraController*)source;
    rotation_ = camCont->rotation_;
    last_mouse_pos_ = camCont->last_mouse_pos_;
  }

}
