#pragma once


#include <iostream>
namespace Reflection
{
  struct LuaMemberdata;
  struct LuaChange
  {
    LuaChange();
    LuaChange(LuaMemberdata* member, char* change);
    LuaMemberdata* member;
    char data[50];
  };

  class LuaChangeList
  {
    public:
      
      void CommitChange(LuaMemberdata* member, char* change);

      void ForceChange(LuaMemberdata* member, char* change);

      void PushChanges();

    private:
      std::unordered_map<LuaMemberdata*, LuaChange> changes_;
      void* target_;
  };
}

