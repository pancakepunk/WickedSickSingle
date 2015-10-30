#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include "meta/Lua/LuaManager.h"
#include "meta/Lua/LuaChangeList.h"

struct lua_State;
namespace Reflection
{
  
  class Script
  {
    public:
      Script(const std::string& file);
      ~Script();

      template<typename T>
      void AddScriptArg(const T& arg)
      {
        Metadata* type = FindType(T);
        
        //search through args and see if it exists
        for(auto& it : script_args_)
        {
          if(it->GetData() == &arg)
          {
            //duplicate
            __debugbreak();
            return;
          }
        }

        typedef RemoveReferences(T) TypeFinal;
        const unsigned short indirection = IndirectionCount<TypeFinal>::value;
        if (FindType(TypeFinal)->GetEnumConstants().size())//set type for enum
        {
          type = FindType(int);
        }

        TypeFinal* argPtr = const_cast<TypeFinal*>(&arg);
        Var* argVar = new Var(*argPtr);
        
        //deprecated methinks
        /*if(type->IsBuiltInType())
        {
          if(type->GetName() == "bool")
          {
            argVar = new Var(*reinterpret_cast<LuaBool*>(const_cast<TypeFinal*>(argPtr)));
          }
          else if(type->GetName() == "long")
          {
            argVar = new Var(*reinterpret_cast<LuaLong*>(const_cast<TypeFinal*>(argPtr)));
          }
          else if(type->GetName() == "short")
          {
            argVar = new Var(*reinterpret_cast<LuaShort*>(const_cast<TypeFinal*>(argPtr)));
          }
          else if(type->GetName() == "int")
          {
            argVar = new Var(*reinterpret_cast<LuaInt*>(const_cast<TypeFinal*>(argPtr)));
          }
          else if(type->GetName() == "unsigned")
          {
            argVar = new Var(*reinterpret_cast<LuaUnsigned*>(const_cast<TypeFinal*>(argPtr)));
          }
          else if(type->GetName() == "float")
          {
            argVar = new Var(*reinterpret_cast<LuaFloat*>(const_cast<TypeFinal*>(argPtr)));
          }
          else if(type->GetName() == "double")
          {
            argVar = new Var(*reinterpret_cast<LuaDouble*>(const_cast<TypeFinal*>(argPtr)));
          }
          else if(type->GetName() == "char")
          {
            if (indirection == 1)
            {
              argVar = new Var(*reinterpret_cast<LuaString*>(const_cast<TypeFinal*>(argPtr)));
            }
            else if (indirection == 0)
            {
              argVar = new Var(*reinterpret_cast<LuaChar*>(const_cast<TypeFinal*>(argPtr)));
            }
          }
        }
        else
        {
          //script_args_.push_back(new Var(*const_cast<TypeFinal*>(&arg)));
          argVar = new Var(*const_cast<TypeFinal*>(argPtr))
        }*/
        argVar->SetLevelsOfIndirection(indirection);
        script_args_.push_back(argVar);

      }

      void ClearScriptArgs();
      bool Load();
      void Unload();
      //bool Run(lua_State* L = Engine::ENGINE->GetSystem<LuaManager>()->GetLua());
      std::vector<Var*>& GetScriptArgs(){return script_args_;}
      std::string& GetFilename(){return filename_;}
      //void PushArgs(lua_State* L = Engine::ENGINE->GetSystem<LuaManager>()->GetLua());

      std::unordered_map<LuaUserdata*, LuaChangeList>& GetChanges(){return ref_changes_;}


    private:
      
      std::string filename_;
      std::string file_;
      std::vector<Var*> script_args_;
      std::unordered_map<LuaUserdata*, LuaChangeList> ref_changes_;
  };
}
