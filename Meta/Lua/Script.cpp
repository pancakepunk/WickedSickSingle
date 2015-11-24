#include "Precompiled.h"
//
#include "meta/Lua/Script.h"

#include "meta/Lua/LuaManager.h"
#include "meta/Variant/Variant.h"
#include "meta/Type/Metadata.h"

namespace Reflection
{
  Script::Script(const std::string& file) : filename_(file)
  {
  }

  Script::~Script()
  {
    ClearScriptArgs();
  }

  void Script::ClearScriptArgs()
  {
    for(auto it : script_args_)
    {
      if(it->GetMetadata()->GetName() != "Engine")
      {
        delete it;
      }
    }

    if(!script_args_.empty())
    {
      script_args_.clear();
    }
  }
  /*
  bool Script::Run(lua_State* L)
  {
    unsigned originalSize = script_args_.size();
    GET_SYSTEM(LuaManager)->set_current_script(this);

    if(file_.size())
    {
      if(luaL_dostring(L, (file_).c_str()))
      {
        Print("%s : %s\n", filename_.c_str(), lua_tostring(L, -1));
        GET_SYSTEM(LuaManager)->end_script();
        return false;
      }

      //set all of the references
      while(script_args_.size() > originalSize)
      {
        if(script_args_.back()->GetMetadata()->GetName() != "Engine")
        {
          delete script_args_.back();
        }
        script_args_.pop_back();
      }
      GET_SYSTEM(LuaManager)->end_script();
      return true;
    }
    
    GET_SYSTEM(LuaManager)->end_script();
    return false;
  }
  */
  bool Script::Load()
  {
    std::ifstream in(filename_.c_str());
    if(in)
    {
      std::string stdstring((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
      file_.assign(stdstring.c_str());
      if(!file_.size())
      {
        __debugbreak();
        //WSError("Lua Warning!\nLoaded empty script: %s" + filename_);
      }
      
      return true;
    }
    __debugbreak();
    //WSError("Lua Warning!\nProblem loading script: %s" + filename_);
    return false;
  }

  void Script::Unload()
  {
    file_.clear();
  }
  /*
  void Script::PushArgs(lua_State* L)
  {
    Metadata* meta;
    
    for(auto& it : script_args_)
    {
      meta = it->GetMetadata();
      if(meta->IsBuiltInType())
      {
        std::string name = meta->GetName();
        if(name == "bool")
        {
          lua_pushboolean(L, it->GetValue<bool>());
        }
        else if(name == "long")
        {
          lua_pushinteger(L, it->GetValue<long>());
        }
        else if(name == "int")
        {
          lua_pushinteger(L, it->GetValue<int>());
        }
        else if(name == "unsigned")
        {
          lua_pushinteger(L, it->GetValue<unsigned>());
        }
        else if(name == "float")
        {
          lua_pushnumber(L, it->GetValue<float>());
        }
        else if(name == "double")
        {
          lua_pushnumber(L, it->GetValue<double>());
        }
        else if(name == "char")
        {
          if(it->GetIndirection() == 1)
          {
            lua_pushstring(L, reinterpret_cast<const char*>(it->GetData()));
          }
        }
      }
      else
      {
        //set object
        LuaUserdata* newdata = (LuaUserdata*)lua_newuserdata(L, sizeof(LuaUserdata) + meta->GetSize());
        memset(newdata, 0, sizeof(LuaUserdata) + meta->GetSize());
        newdata->SetObject(meta, it->GetData());
        newdata->SetIndirection(it->GetIndirection());

        //set meta table
        luaL_getmetatable(L, meta->GetName().c_str());
        lua_setmetatable(L, -2);
      }
    }

    script_args_.push_back(GET_SYSTEM(LuaManager)->GetEngineRef());
  }
  */

}
