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
    if (handler->Check("Rotate"))
    {
      if(last_mouse_pos_ == Vector2i(-1, -1))
      {
        last_mouse_pos_ = handler->GetMousePos();
      }
      else if(handler->GetMousePos() != last_mouse_pos_)
      {
        Vector2i curPos = handler->GetMousePos();
        Vector2 amountToRotate = (curPos - last_mouse_pos_).to_f() * dt;
        Vector3 lookAt = cameraComp->GetLookAt();
        Transform* tr = (Transform*)GetSibling(CT_Transform);
        Vector3 view = lookAt - tr->GetPosition();
        view.Normalize();
        
        
        view = RotateAround(Vector3(0, 1, 0), amountToRotate.x, view);
        view.Normalize();
        Vector3 right = Vector3(0,1,0).Cross(view);
        right.Normalize();
        view = RotateAround(right, amountToRotate.y, view);

        
        lookAt = (tr->GetPosition() + view);


        cameraComp->SetLookAt(lookAt);
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
        Vector2 amountToMove = (curPos - last_mouse_pos_).to_f() * dt;
        
        Vector3 lookAt = cameraComp->GetLookAt();
        Transform* tr = (Transform*)GetSibling(CT_Transform);

        Vector3 view = lookAt - tr->GetPosition();
        view.Normalize();
        Vector3 right = Vector3(0.0f,1.0f,0.0f).Cross(view);
        right.Normalize();
        Vector3 up = view.Cross(right);
        up.Normalize();

        Vector3 pos = tr->GetPosition();
        amountToMove *= 5.0f;
        pos += right * -amountToMove.x;
        pos += up * amountToMove.y;

        lookAt += right * -amountToMove.x;
        lookAt += up * amountToMove.y;
        tr->SetPosition(pos);
        cameraComp->SetLookAt(lookAt);

        last_mouse_pos_ = Vector2i(-1, -1);
        dirty = true;
      }
    }
    else if (handler->Check("ZoomIn"))
    {
      Vector3 lookAt = cameraComp->GetLookAt();
      Transform* tr = (Transform*)GetSibling(CT_Transform);
      Vector3 view = lookAt - tr->GetPosition();
      view.Normalize();
      view *= dt;
      tr->SetPosition(tr->GetPosition() += view );
      cameraComp->SetLookAt(lookAt += view );
      dirty = true;
    }
    else if (handler->Check("ZoomOut"))
    {
      Vector3 lookAt = cameraComp->GetLookAt();
      Transform* tr = (Transform*)GetSibling(CT_Transform);
      Vector3 view = lookAt - tr->GetPosition();
      view.Normalize();
      view *= dt;
      tr->SetPosition(tr->GetPosition() -= view);
      cameraComp->SetLookAt(lookAt -= view);
      dirty = true;
    }
    else if (handler->GetScrollPos())
    {
      Vector3 lookAt = cameraComp->GetLookAt();
      Transform* tr = (Transform*)GetSibling(CT_Transform);
      Vector3 view = lookAt - tr->GetPosition();
      view.Normalize();
      view *= dt;
      tr->SetPosition(tr->GetPosition() + view * (float)handler->GetScrollPos());
      cameraComp->SetLookAt(lookAt + view * (float)handler->GetScrollPos());
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

}
