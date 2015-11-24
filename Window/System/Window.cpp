#include "Precompiled.h"
#include "WindowPrecompiled.h"
#include "Window.h"

#include "EventSystem/EventSystemInterface.h"
#include "Input/InputInterface.h"
#include "Core/CoreInterface.h"

#include "anttweakbar/include/AntTweakBar.h"

namespace WickedSick
{
  struct WinMsg
  {
    HWND hWnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
  };

  LRESULT CALLBACK WindowProc(HWND hWnd,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam)
  {
    static std::queue<WinMsg> backlog;
    if(!Engine::IsIniitialized())
    {
      backlog.push({hWnd, message, wParam, lParam});
    }
    else if(backlog.size())
    {
      while(backlog.size())
      {
        WinMsg& top = backlog.front();
        TwEventWin(top.hWnd, top.message, top.wParam, top.lParam);
        backlog.pop();
      }
      
    }
    else
    {
      if(TwEventWin(hWnd, message, wParam, lParam))
      {
        return 1;
      }
    }
    
    
    EventSystem* msgSystem = Engine::GetCore()->GetSystem<EventSystem>(ST_EventSystem);
    Window* windowSystem = Engine::GetCore()->GetSystem<Window>(ST_Window);
    InputBuffer* buf = windowSystem->GetInputBuffer();

    Vector2i pos;
    pos.x = GET_X_LPARAM(lParam);
    pos.y = GET_Y_LPARAM(lParam);

    POINT point;
    point.x = pos.x;
    point.y = pos.y;
    
    ScreenToClient(hWnd, &point);

    Vector2i mousePos;
    mousePos.x = point.x;
    mousePos.y = point.y;


    // sort through and find what code to run for the message given
    if(!windowSystem->InFocus())
    {
      if(message != WM_SETFOCUS)
      {
        return DefWindowProc (hWnd, message, wParam, lParam);
      }
    }

    switch(message)
    {
        ///////////////////////////////////////////////////////////////////////////
      // Handle Cursor
      ///////////////////////////////////////////////////////////////////////////
      
      case WM_MOUSEWHEEL:
      {
        
        buf->wheelChange = (GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? +1 : -1;
        //buf->wheelChange = (wParam / WHEEL_DELTA);
        break;
      }
      case WM_MOUSEMOVE:
      {
        buf->mousePos = mousePos;
        TwMouseMotion(mousePos.x, mousePos.y);

        break;
      }
      case WM_LBUTTONUP:
      {
        buf->mousePos = mousePos;
        buf->down[VK_LBUTTON] = false;
        TwMouseMotion(mousePos.x, mousePos.y);
        break;
      }
      case WM_RBUTTONUP:
      {
        buf->mousePos = mousePos;
        buf->down[VK_RBUTTON] = false;
        TwMouseMotion(mousePos.x, mousePos.y);
        break;
      }
      case WM_MBUTTONUP:
      {
        buf->mousePos = mousePos;
        buf->down[VK_MBUTTON] = false;
        TwMouseMotion(mousePos.x, mousePos.y);
        break;
      }
      case WM_LBUTTONDOWN:
      {
        buf->mousePos = mousePos;
        buf->down[VK_LBUTTON] = true;
        TwMouseMotion(mousePos.x, mousePos.y);
        break;
      }
      case WM_RBUTTONDOWN:
      {
        buf->mousePos = mousePos;
        buf->down[VK_RBUTTON] = true;
        TwMouseMotion(mousePos.x, mousePos.y);
        break;
      }
      case WM_MBUTTONDOWN:
      {
        buf->mousePos = mousePos;
        buf->down[VK_MBUTTON] = true;
        TwMouseMotion(mousePos.x, mousePos.y);
        break;
      }
      case WM_MOUSELEAVE: //this doesn't seem to do anything
      {
        //noperino
        //MouseEvent* mouse = new MouseEvent(ST_Window, ST_Input, Vector2i(), MouseEvent::Exited);
        //msgSystem->QueueEvent(mouse);
        break;
      }

      ///////////////////////////////////////////////////////////////////////////
      // Focus Messages
      ///////////////////////////////////////////////////////////////////////////
      case WM_KILLFOCUS:
      {
        //Engine::ENGINE->Pause();
        //sound
        //FMOD::ChannelGroup* master;
        //soundSys->GetSystem()->getMasterChannelGroup(&master);
        //soundSys->Pause();
        ////

        break;
      }
    
      case WM_SETFOCUS:
      {
        //sound
        //FMOD::ChannelGroup* master;
        //soundSys->GetSystem()->getMasterChannelGroup(&master);
        //master->setVolume(Engine::ENGINE->GetSettings()->sound.MasterVolume);
        //if(!Engine::ENGINE->IsPaused())
        //{
        //  soundSys->UnPause();
        //}
        ////
      


        break;
      }
    

      ///////////////////////////////////////////////////////////////////////////
      // Handle Keys
      ///////////////////////////////////////////////////////////////////////////
      case WM_KEYDOWN:
      {
        buf->down[wParam] = true;
        switch(wParam)
        {
          case VK_LCONTROL:
          case VK_RCONTROL:
          case VK_CONTROL:
            buf->current_modifiers_[ModifierType::Control] = true;
            break;
          case VK_LMENU:
          case VK_RMENU:
          case VK_MENU:
            buf->current_modifiers_[ModifierType::Alt] = true;
            break;
          case VK_LSHIFT:
          case VK_RSHIFT:
          case VK_SHIFT:
            buf->current_modifiers_[ModifierType::Shift] = true;
            break;
        }
        break;
      }
      case WM_KEYUP:
      {
        buf->down[wParam] = false;
        switch (wParam)
        {
          case VK_LCONTROL:
          case VK_RCONTROL:
          case VK_CONTROL:
            buf->current_modifiers_[ModifierType::Control] = false;
            break;
          case VK_LMENU:
          case VK_RMENU:
          case VK_MENU:
            buf->current_modifiers_[ModifierType::Alt] = false;
            break;
          case VK_LSHIFT:
          case VK_RSHIFT:
          case VK_SHIFT:
            buf->current_modifiers_[ModifierType::Shift] = false;
            break;
        }
        break;
      }
      ///////////////////////////////////////////////////////////////////////////
      // Handle Resize
      ///////////////////////////////////////////////////////////////////////////
      case WM_SIZE:
      {

        RECT r;
        GetClientRect(hWnd, &r);
        TwWindowSize(r.right - r.left, r.bottom - r.top);
        //if(window->IsFullscreen())
        //{
        //  data.m_window_size = data.m_desktop_size;
        //  data.m_viewport_size = data.m_desktop_size;
        //}
        //else
        //{
        //  RECT wind = { 0, 0, data.m_target_size.x, 
        //                      data.m_target_size.y };
        //  if(GetClientRect(window->GetHandle(), &wind))
        //  {
        //    //adjust window for padding
        //    RECT correctW = { 0, 0, 
        //                      data.m_target_size.x, 
        //                      data.m_target_size.y };
        //    AdjustWindowRect(&correctW, WS_OVERLAPPEDWINDOW, FALSE); 
        //    int VPw = correctW.right - correctW.left;
        //    int VPh = correctW.bottom - correctW.top;
        //    data.m_window_size.Set(VPw, VPh);
        //  }
        //  else
        //  {
        //    //Failed to get client rect?

        //    data.m_renderable_window_size = data.m_target_size;

        //    //adjust window for padding
        //    RECT correctW = { 0, 0, 
        //                      data.m_target_size.x, 
        //                      data.m_target_size.y };
        //    AdjustWindowRect(&correctW, WS_OVERLAPPEDWINDOW, FALSE); 
        //    int VPw = correctW.right - correctW.left;
        //    int VPh = correctW.bottom - correctW.top;
        //    data.m_window_size.Set(VPw, VPh);
        //  }
        //}
        //window->ResizeD3D();
        break;
      }
      ///////////////////////////////////////////////////////////////////////////
      // Handle Destruction
      ///////////////////////////////////////////////////////////////////////////
      case WM_DESTROY:
      case WM_CLOSE:
      case WM_QUIT:
      {
        msgSystem->QueueEvent("Quit");
        //Event* quit = new Event(ST_Window, GlobalID, WickedSick::Event::Quit);
        //msgSystem->QueueEvent(quit);
        break;
      }
      case WM_SYSKEYDOWN:
      {
        switch(wParam)
        {
          case VK_F4:
          {
            //Event* quit = new Event(ST_Window, GlobalID, WickedSick::Event::Quit);
            //msgSystem->QueueEvent(quit);
            break;         
          }
          case VK_RETURN:
          {
            //Event quit(System::Window, GlobalID, WickedSick::Event::Quit);
            break;
          }
          case VK_TAB:
          {
            //if(window->IsFullscreen())
            //{
              //ShowWindow(window->GetHandle(), SW_MINIMIZE);
            //}
            break;
          }
          default:
            break;
        }
        break;
      }
      default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
    // Handle any messages the switch statement didn't
    return 1;
  }

  Window::Window() : System(ST_Window)
  {
    window_instance_ = GetModuleHandle(nullptr);
    input_buf_ = new InputBuffer();
  }

  Window::~Window()
  {
    delete input_buf_;
  }

  void Window::Initialize()
  {
    in_focus_ = true;
    //DefWindowProc(hwnd, uMsg, wParam, lParam);
    WNDCLASSEX windowInfo;
    ZeroMemory(&windowInfo, sizeof(windowInfo));
    
    windowInfo.cbSize = sizeof(windowInfo);
    windowInfo.style = CS_HREDRAW | CS_VREDRAW;
    windowInfo.lpfnWndProc = WindowProc;
    windowInfo.hInstance = window_instance_;
    windowInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowInfo.hbrBackground = (HBRUSH)COLOR_WINDOW;
    windowInfo.lpszClassName = "Classy";

    RegisterClassEx(&windowInfo);

    DEVMODE curMode, bestMode;
    curMode.dmSize = sizeof(curMode);
    int modeNumber = 0;
    
    int retVal;
    retVal = EnumDisplaySettings(NULL, modeNumber, &curMode);
    bestMode = curMode;

    while (retVal)
    {
      ++modeNumber;
      if (curMode.dmBitsPerPel >= bestMode.dmBitsPerPel)
      {
        if (  (curMode.dmPelsWidth  >= bestMode.dmPelsWidth ) &&
              (curMode.dmPelsHeight >= bestMode.dmPelsHeight)  )
        {
          bestMode = curMode;
        }
      }
      retVal = EnumDisplaySettings(NULL, modeNumber, &curMode);
    }

    window_size_.x = bestMode.dmPelsWidth;
    window_size_.y = bestMode.dmPelsHeight;

    

    std::lock_guard<std::mutex> handleLock(handle_mutex_);
    window_handle_ = CreateWindowEx(WS_EX_APPWINDOW,        //window type
                                    "Classy",              //window class name?
                                    "Wicked Sick Engine",  //window name
                                    WS_OVERLAPPEDWINDOW,    //also window type?
                                    0,                      //start position x
                                    0,                      //start position y
                                    bestMode.dmPelsWidth,   //width
                                    bestMode.dmPelsHeight,  //height
                                    nullptr,                //parent window ptr
                                    nullptr,                //menu ptr
                                    windowInfo.hInstance,   //got this from GetModuleHandle() earlier
                                    nullptr);               //used with multiple windows?
    ShowWindow(window_handle_, SW_SHOW);
    RECT r;
    GetClientRect(window_handle_, &r);
    window_size_.x = r.right - r.left;
    window_size_.y = r.bottom - r.top;

  }
  
  bool Window::Load()
  {
    return true;
  }
  
  bool Window::Reload()
  {
    return true;
  }
  
  void Window::Update(float dt)
  {
    input_buf_->wheelChange = 0;
    Input* inputSystem = Engine::GetCore()->GetSystem<Input>(ST_Input);

    message_update();
    inputSystem->UpdateInput(input_buf_);

    /*ImGuiIO& imgui = ImGui::GetIO();
    memcpy(imgui.KeysDown, input_buf_->down, sizeof(bool) * 256);
    

    imgui.MouseClicked[0] = input_buf_->down[VK_LBUTTON];
    imgui.MouseClicked[1] = input_buf_->down[VK_RBUTTON];
    imgui.MouseClicked[2] = input_buf_->down[VK_MBUTTON];*/
    
    
  }

  void Window::ReceiveMessage(Event * msg)
  {

  }

  HWND& Window::GetWindowHandle()
  {
    return window_handle_;
  }

  InputBuffer * Window::GetInputBuffer()
  {
    return input_buf_;
  }


  std::mutex& Window::GetWindowHandleMutex()
  {
    return handle_mutex_;
  }

  Vector2i Window::GetWindowSize()
  {
    return window_size_;
  }

  bool Window::InFocus()
  {
    return in_focus_;
  }


  void Window::message_update()
  {

    BOOL msgErr;
    do
    {
      msgErr = PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE);
      if (msgErr)
      {
        //switch (msg_.message)
        //{
        //default:
        //  break;
        //}
        TranslateMessage(&msg_);
        DispatchMessage(&msg_);
      }
    }
    while (msgErr);
  }
}
