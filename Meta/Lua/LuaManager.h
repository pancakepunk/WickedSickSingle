#pragma once
#include <iostream>
#include <vector>
#include "meta/Lua/LuaUserdata.h"
#include "meta/Lua/LuaIncludes.h"
#include "meta/Utility/MetaMacros.h"

typedef int (*lua_CFunction) (lua_State *L);

namespace Reflection
{
  class Metadata;
  class Script;
  class ScriptManager;
  //in order to call a function from lua, we must be able to convert it to this type:
  typedef int (*lua_func) (lua_State *L, void *v);

  //THIS DOES NOT COME FROM LUA.
  struct luaL_MemberReg
  {
    const char* name;
    lua_func func;
    size_t offset;
  };
  
  class Var;

  class LuaManager
  {
    public:
      MetaDef;
      LuaManager();
      ~LuaManager();
      bool Load();
      
      lua_State* GetLua();
      


      Script* GetCurrentScript();
      ScriptManager* GetScriptManager();
      
      Script* GetScript(const std::string& filename);
      Script* AddScript(const std::string& filename);
      void RunScript(const std::string& filename);

      void set_current_script(Script* s);

      void end_script();
    private:

      ScriptManager* manager_;
      Script* current_script_;
      std::vector<Script*> current_scripts_;
      lua_State* lua_;
  };

  namespace Lua
  {
    static int call(lua_State *L);
    static int index_handler(lua_State *L);
    static int newindex_handler(lua_State *L);

    static int RefCast(lua_State* L);
    static int GetArgs(lua_State* L);
    static int Allocate(lua_State* L);
    static int GenericFunc(lua_State* L);
    static int GetMember(lua_State* L);
    static int access_member(lua_State* L,
                             void* data,
                             LuaMemberdata* memberData);
    static void lua_heap_construct(LuaUserdata* data);

    static void construct(lua_State* L,
                          const std::string& ctorName,
                          Metadata* type,
                          std::vector<Var*>& args,
                          bool heap);


    static const std::string* find_constructor(lua_State* L,
                                               Metadata* type,
                                               std::vector<Var*>& args);
    static const char* find_type_name(lua_State* L);
    static const char* find_function_name(lua_State* L);

    static void add(lua_State *L, 
                    std::vector<LuaMemberdata>& l);
  }

}
