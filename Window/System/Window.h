#pragma once

#include "Core/CoreInterface.h"

#include "Math/MathInterface.h"


#include "Window/General/WindowSettings.h"

#include <mutex>

//#include <windows.h>
namespace WickedSick
{
  LRESULT CALLBACK WindowProc(HWND hWnd,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam);
  struct InputBuffer;
  class Window : public System
  {
  public:
    Window();
    ~Window();
    void Initialize();
    bool Load();
    bool Reload();
    void Update(float dt);
    void ReceiveMessage(Event * msg);
    HWND& GetWindowHandle();
    InputBuffer* GetInputBuffer();
    Vector2i GetWindowSize();
    bool InFocus();

    std::mutex& GetWindowHandleMutex();
  private:

    void message_update();
    InputBuffer* input_buf_;
    friend LRESULT CALLBACK WindowProc(HWND hWnd,
                                       UINT message,
                                       WPARAM wParam,
                                       LPARAM lParam);

    Vector2i window_size_;
    
    bool in_focus_;

    MSG msg_;
    HWND window_handle_;
    HINSTANCE window_instance_;

    std::mutex handle_mutex_;

    WindowSettings settings_;
  };
}
