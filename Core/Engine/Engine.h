#pragma once

#include "System/System.h"
#include <vector>


namespace WickedSick
{
  //predeclarations
  class System;
  class FrameController;


  class Engine
  {
  public:
    Engine();
    ~Engine();
    void AddSystem(System* sys);
    
    template <typename T>
    T* GetSystem(SystemType type)
    {
      return reinterpret_cast<T*>(systems_[type]);
    }

    System* GetSystem(SystemType type);
    std::vector<System*>& GetSystems();

    static bool IsIniitialized()
    {
      return initialized_;
    }

    void Initialize();
    void Load();
    void Reload();
    void Update();

    bool IsActive();
    void ShutDown();

    static Engine* GetCore()
    {
      return Core;
    }

    static Engine* Core;

  private:
    std::vector<System*>  systems_;
    FrameController*      frame_controller_;
    bool                  active_;
    static bool                  initialized_;
  };
}
