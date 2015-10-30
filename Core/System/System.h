#pragma once



namespace WickedSick
{
  //predeclarations
  class Engine;
  class Event;


  enum SystemType
  {
    #define RegisterSystem(x) ST_##x,
    #include "RegisterSystem.h"
    #undef RegisterSystem
    ST_Count
  };

  class System
  {

    public:

      System(SystemType type);
      ~System();
      void SetCore(Engine* core);
      Engine* GetCore() const;
      SystemType GetType();
      virtual void  Initialize() = 0;
      virtual bool  Load() = 0;
      virtual bool  Reload() = 0;
      virtual void  Update(float dt) = 0;
      virtual void  ReceiveMessage(Event* msg) = 0;

      

    #define GlobalID ST_Count

    protected:
      SystemType    type_;
      Engine*       core_;
  };
}
