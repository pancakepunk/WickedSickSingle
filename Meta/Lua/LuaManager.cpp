#include "Precompiled.h"


#include "meta/Lua/LuaManager.h"
#include "meta/Lua/LuaIncludes.h"
#include "meta/Lua/Script.h"
#include "meta/Lua/ScriptManager.h"
#include "meta/Lua/LuaUserdata.h"
#include "meta/Type/Metadata.h"
#include "meta/Type/MetaManager.h"
#include "meta/Utility/IndirectionCount.h"
#include "meta/Function/Function.h"
#include "meta/Definitions/Primitives.h"

namespace Reflection
{
  static int l_my_print(lua_State* L) 
  {
    int nargs = lua_gettop(L);

    for (int i=1; i <= nargs; i++) 
    {
      if (lua_isstring(L, i)) 
      {
        /* Pop the next arg using lua_tostring(L, i) and do your print */
        std::string str = lua_tostring(L, i);
        std::cout << str << std::endl;
        //ConsolePrint(str);
      }
      else if(lua_isuserdata(L, i))
      {
        LuaUserdata* arg = (LuaUserdata*)alloca(sizeof(LuaUserdata));
        arg = (LuaUserdata*)lua_touserdata(L, lua_gettop(L));
        if(arg->GetType() == FindType(std::string))
        {
          std::string str = *(std::string*)arg->GetData();
          std::cout << str << std::endl;
          //ConsolePrint(str);
        }
        else if(arg->GetType() == FindType(char) && arg->GetIndirection() == 1)
        {
          __debugbreak();//fix this stupid shit
          std::string str = (char*)arg;
        }
      }
    }
    return 0;
  }

  static const struct luaL_Reg printlib [] = 
  {
    {"print", l_my_print},
    {NULL, NULL} /* end of array */
  };

  /*extern int luaopen_luamylib(lua_State *L)
  {
    lua_getglobal(L, "_G");
    luaL_register(L, NULL, printlib);
    lua_pop(L, 1);
  }*/

  int Lua::GetMember(lua_State* L)
  {

    return 1;
  }

  int Lua::GetArgs(lua_State* L)
  {
    //GET_SYSTEM(Lua)->GetCurrentScript()->PushArgs(L);
    return 0;//GET_SYSTEM(Lua)->GetCurrentScript()->GetScriptArgs().size();
  }

  Script* LuaManager::GetScript(const std::string& filename)
  {
    return manager_->GetScript(filename);
  }

  static int Terminate(lua_State* L)
  {
    return luaL_error(L, "shit");
  }

  void Lua::lua_heap_construct(LuaUserdata* data)
  {
    void* newObject = (void*)new char[data->GetType()->GetSize()];

    Var* newObjectRef = new Var();
    newObjectRef->SetData(newObject);
    newObjectRef->SetType(data->GetType());
    newObjectRef->SetOwns(true);
    newObjectRef->Validate();

    Metadata* type = data->GetType();
    std::vector<Var*> args;
    Var self(data);
    self.SetType(data->GetType());
    args.push_back(&self);
    Constructor* ctor = type->GetConstructor("copy");
    if(!ctor)
    {
      __debugbreak();
      //WSError("No copy constructor defined or no constructor exists...")
    }

    (*ctor)(newObjectRef, args);
    newObjectRef->SetOwns(true);
    //need a way of telling the meta system about this new object.... this is not the way
    //data->SetRef(newObject);

    //GET_SYSTEM(Lua)->GetCurrentScript()->GetScriptArgs().push_back(newObjectRef);

    //GET_SYSTEM(Lua)->GetCurrentScript()->GetReferences()[newObjectRef] = data;
  }

  const std::string* Lua::find_constructor(lua_State* L, Metadata* type, std::vector<Var*>& args)
  {
    const std::string* match = nullptr;

    std::vector<Metadata*>::iterator ctorArgIt;
    std::vector<Var*>::iterator argIt;
    bool noMatch;
    for(auto it : type->GetConstructors())//loop through all constructors of the type
    {
      if(args.size())// only come through here if we have arguments
      {
        argIt = args.begin();
        ctorArgIt = it.second.GetArgs().begin();
        //loops through both arg lists, compares to make sure types convert / match
        noMatch = false;
        while((ctorArgIt != it.second.GetArgs().end()) && (argIt != args.end()))
        {
          if(!((*argIt)->GetMetadata()->Converts(*ctorArgIt))) // doesn't match now
          {
            noMatch = true;
            break;
          }
          ++ctorArgIt;
          ++argIt;
        }
        if(noMatch)
        {
          continue;
        }
      }
      else if(it.second.GetArgs().size())// no args, and this ctor has args
      {
        continue; // don't add the name
      }
      if(!match)
      {
        match = &(it.first);
      }
      else
      {
        std::string errorString = "Call to constructor of type " + type->GetName() + " ambiguous. ";
        luaL_error(L, errorString.c_str());
      }
    }

    //we didn't find any matches
    if(!match)
    {
      std::string errorString = "No constructor exists for type " + type->GetName() + " with argument types:";
      for(auto it : args)
      {
        errorString.append("\n" + it->GetMetadata()->GetName());
      }
      luaL_error(L, errorString.c_str());
    }

    return match;
  }

  const char* Lua::find_type_name(lua_State* L)
  {
    //this bit of code is for finding our type name from lua
    const char* toreturn = "";
    lua_pushglobaltable(L);
    for(lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1))
    {
      if(lua_rawequal(L, -1, -4))
      {
        toreturn = lua_tostring(L, -2);
        lua_pop(L, 3);
        break;
      }
    }

    if(!strlen(toreturn))// type not bound? this shouldn't actually happen.
    {
      std::string errorString = "Couldn't find type in constructor call.";
      luaL_error(L, errorString.c_str());
    }

    return toreturn;
  }

  const char* Lua::find_function_name(lua_State* L)
  {
    const char* toreturn = "";
    // get the function name from lua debug
    lua_Debug entry;
    lua_getstack(L, 0, &entry);
    lua_getinfo(L, "Sln", &entry);
    toreturn = entry.name;
    lua_pop(L, 1);
    return toreturn;
  }

  void Lua::construct(lua_State* L, 
                      const std::string& ctorName, 
                      Metadata* type, 
                      std::vector<Var*>& args, 
                      bool heap)
  {
    //create userdata for the return type
    LuaUserdata* data = (LuaUserdata*)lua_newuserdata(L, sizeof(LuaUserdata) + type->GetSize());
    memset(data, 0, sizeof(LuaUserdata) + type->GetSize());
    data->SetType(type);

    //set buffer and type for variant
    Var returnValue(data);
    returnValue.SetType(type);


    //construct the type on top of the LuaUserdata
    (*type->GetConstructor(ctorName))(&returnValue, args);
    

    //set the metatable
    luaL_getmetatable(L, type->GetName().c_str());
    lua_setmetatable(L, -2);

    //if the heap flag was set we construct it on the heap as well
    if(heap)
    {
      lua_heap_construct(data);
    }
  }

  int Lua::call(lua_State * L)
  {
    /* for get: stack has userdata, index, lightuserdata */
    /* for set: stack has userdata, index, value, lightuserdata */
    LuaMemberdata* m = (LuaMemberdata*)lua_touserdata(L, -1);  /* member info */
    lua_pop(L, 1);                               /* drop lightuserdata */
    luaL_checktype(L, 1, LUA_TUSERDATA);
    LuaUserdata* userdata = (LuaUserdata*)lua_touserdata(L, 1);
    //ErrorIf(userdata->GetIndirection() != m->indirection, "META INDIRECTION CONFLICT");
    
    if(userdata->GetIndirection() != m->indirection)
      __debugbreak();
    void* dataPtr = userdata->GetData();
    if (m->indirection)
    {
      dataPtr = blind_dereference(dataPtr, m->indirection);
      if (!dataPtr)
      {
        lua_pushnil(L);
        return 1;
      }
    }
    int toreturn = access_member(L, (void*)((char*)dataPtr + m->offset), m);

    return toreturn;
  }

  void Lua::add(lua_State * L, std::vector<LuaMemberdata>& l)
  {
    for (auto& it : l)
    {
      lua_pushstring(L, it.name);
      lua_pushlightuserdata(L, (void*)&it);
      lua_settable(L, -3);
    }
  }

  int Lua::index_handler(lua_State * L)
  {
    /* stack has userdata, index */
    lua_pushvalue(L, 2);                     /* dup index */
    lua_rawget(L, lua_upvalueindex(1));      /* lookup member by name */
    if (!lua_islightuserdata(L, -1))
    {
      lua_pop(L, 1);                         /* drop value */
      lua_pushvalue(L, 2);                   /* dup index */
      lua_gettable(L, lua_upvalueindex(2));  /* else try methods */
      if (lua_isnil(L, -1))                  /* invalid member */
        luaL_error(L, "cannot get member '%s'", lua_tostring(L, 2));
      return 1;
    }
    return call(L);                      /* call get function */
  }

  int Lua::newindex_handler(lua_State * L)
  {
    /* stack has userdata, index, value */
    lua_pushvalue(L, 2);                     /* dup index */
    lua_rawget(L, lua_upvalueindex(1));      /* lookup member by name */
    if (!lua_islightuserdata(L, -1))         /* invalid member */
      luaL_error(L, "cannot set member '%s'", lua_tostring(L, 2));
    return call(L);                      /* call set function */
  }

  int Lua::Allocate(lua_State* L)
  {

    int argc = lua_gettop(L);
    int returnValues = 0;
    std::vector<Var*> args;

    //heap flag
    bool heap = (lua_toboolean(L, lua_gettop(L)) != 0);
    --argc;
    lua_pop(L, 1);


    if(argc > 1)// we have arguments passed
    {
      //predeclare our pointers
      LuaUserdata* arg = nullptr;
      Var* newref;
      //loop through and grab the arguments
      for(int i = 1; i < argc; ++i)
      { 
        //allocate the reference and luadata on the stack
        newref = (Var*)alloca(sizeof(Var));
        memset(newref, 0, sizeof(Var));
        switch(lua_type(L, lua_gettop(L)))
        {
          case LUA_TBOOLEAN:
          {
            // grab boolean value, set our luadata
            bool copyFrom = (lua_toboolean(L, lua_gettop(L)) != 0);
            arg = (LuaUserdata*)alloca(sizeof(LuaUserdata) + sizeof(bool));//its a bool so
            memset(arg, 0, sizeof(LuaUserdata) + sizeof(bool));
            arg->SetObject(FindType(bool), &copyFrom);
            break;
          }
          case LUA_TNUMBER:
          {
            // grab double value, set our luadata
            double copyFrom = lua_tonumber(L, lua_gettop(L));
            arg = (LuaUserdata*)alloca(sizeof(LuaUserdata) + sizeof(double));
            memset(arg, 0, sizeof(LuaUserdata) + sizeof(double));
            arg->SetObject(FindType(double), &copyFrom);
            break;
          }
          case LUA_TSTRING:
          {
            // grab std::string value, set our luadata
            const char* copyFrom = lua_tostring(L, lua_gettop(L));
            arg = (LuaUserdata*)alloca(sizeof(LuaUserdata) + strlen(copyFrom) + 1);
            memset(arg, 0, sizeof(LuaUserdata) + strlen(copyFrom) + 1);
            memcpy(arg->GetData(), copyFrom, strlen(copyFrom) + 1);
            
            arg->SetType(FindType(char));
            arg->SetIndirection(1);
            
            break;
          }
          case LUA_TUSERDATA:
          {
            // grab luadata, we can just overwrite the luadata pointer for this
            arg = (LuaUserdata*)lua_touserdata(L, lua_gettop(L));
            break;
          }
        }

        //set the refvariant and push it back, pop the argument off the stack
        if(arg == nullptr)
        {
          __debugbreak();
        }

        new (newref) Var(arg);
        args.push_back(newref);
        lua_pop(L, 1);
      }
      std::reverse(args.begin(), args.end());
    }

    std::string type = find_type_name(L);

    Metadata* typeMeta = MetaManager::Get(type);
    
    std::string ctorName = find_function_name(L);

    if(typeMeta)// make sure we actually got the type.
    {
      if(ctorName == "new")// if it's 'new' we need to deduce the correct constructor to use
      {
        //find our real ctor name
        ctorName.assign(*find_constructor(L, typeMeta, args));
      }
      //construct the type
      construct(L, ctorName, typeMeta, args, heap);
    }

    //always returns an object on success
    return 1;
  }

  int Lua::RefCast(lua_State* L)
  {
    int argc = lua_gettop(L);
    std::string targetTypeName;
    LuaUserdata* start;
    targetTypeName.assign(lua_tostring(L, lua_gettop(L)));

    Metadata* targetType = MetaManager::Get(targetTypeName);
    lua_pop(L, 1);
    start = (LuaUserdata*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);
    LuaUserdata* toreturn = (LuaUserdata*)lua_newuserdata(L, sizeof(LuaUserdata) + targetType->GetSize());
    memcpy(toreturn, start, sizeof(LuaUserdata) + targetType->GetSize());
    toreturn->SetType(targetType);
    luaL_getmetatable(L, targetTypeName.c_str());
    lua_setmetatable(L, -2);
    
    return 1;
  }

  int Lua::access_member(lua_State* L, void* data, LuaMemberdata* memberData)
  {
    switch(memberData->type)
    {
      case LuaMemberdata::Get:
      {
        unsigned i = memberData->indirection;
        if(memberData->meta != FindType(char))
        {
          while(i)
          {
            if(data)
            {
              data = (void*)(*(char**)data);
            }
            else
            {
              lua_pushnil(L);
              return 1;
            }
            --i;
          }
          if(!data)
          {
            lua_pushnil(L);
            return 1;
          }
        }
        //check for built in types, lua specific behaviors for such things.

        if(!memberData->meta->GetEnumConstants().empty())
        {
          lua_pushinteger(L, *reinterpret_cast<int*>(data));
        }
        else if(memberData->meta->IsBuiltInType())
        {
          if(memberData->meta->GetName() == "bool")
          {
            lua_pushboolean(L, *reinterpret_cast<bool*>(data));
          }
          else if(memberData->meta->GetName() == "short")
          {
            lua_pushinteger(L, *reinterpret_cast<short*>(data));
          }
          else if(memberData->meta->GetName() == "uint16_t")
          {
            lua_pushinteger(L, *reinterpret_cast<uint16_t*>(data));
          }
          else if(memberData->meta->GetName() == "long")
          {
            lua_pushinteger(L, *reinterpret_cast<long*>(data));
          }
          else if(memberData->meta->GetName() == "int")
          {
            lua_pushinteger(L, *reinterpret_cast<int*>(data));
          }
          else if(memberData->meta->GetName() == "unsigned")
          {
            lua_pushinteger(L, *reinterpret_cast<unsigned*>(data));
          }
          else if(memberData->meta->GetName() == "float")
          {
            lua_pushnumber(L, *reinterpret_cast<float*>(data));
          }
          else if(memberData->meta->GetName() == "double")
          {
            lua_pushnumber(L, *reinterpret_cast<double*>(data));
          }
          else if(memberData->meta->GetName() == "char")
          {
            //todo: check indirection, treat char as int if none
            //lua_pushstring(L, *reinterpret_cast<char**>(data));
            lua_pushstring(L, reinterpret_cast<char*>(data));
          }
        }
        else if(memberData->meta->GetName() == "string")//std::string special case
        {
          LuaUserdata* newdata = (LuaUserdata*)lua_newuserdata(L, sizeof(LuaUserdata) + memberData->meta->GetSize());
          memset(newdata, 0, sizeof(LuaUserdata) + memberData->meta->GetSize());
          newdata->SetObject(memberData->meta, data);


          luaL_getmetatable(L, memberData->meta->GetName().c_str());
          lua_setmetatable(L, -2);
        }
        else
        {
          LuaUserdata* newdata = (LuaUserdata*)lua_newuserdata(L, sizeof(LuaUserdata) + memberData->meta->GetSize());
          memset(newdata, 0, sizeof(LuaUserdata) + memberData->meta->GetSize());
          newdata->SetObject(memberData->meta, data);
          newdata->SetIndirection(memberData->indirection);


          luaL_getmetatable(L, memberData->meta->GetName().c_str());
          lua_setmetatable(L, -2);
        }

        return 1;
      }
      case LuaMemberdata::Set:
      {
        if(memberData->meta->IsBuiltInType()) //built in type cases
        {
          if(memberData->meta->GetName() == "bool")
          {
            *(bool*)data = (lua_toboolean(L, -1) != 0);
          }
          else if(memberData->meta->GetName() == "long")
          {
            *(long*)data = luaL_checkint(L, 3);
          }
          else if(memberData->meta->GetName() == "int")
          {
            *(int*)data = luaL_checkint(L, 3);
          }
          else if(memberData->meta->GetName() == "short")
          {
            *(short*)data = (short)luaL_checkint(L, 3);
          }
          else if(memberData->meta->GetName() == "uint16_t")
          {
            *(uint16_t*)data = (uint16_t)luaL_checkint(L, 3);
          }
          else if(memberData->meta->GetName() == "unsigned")
          {
            *(unsigned*)data = luaL_checkint(L, 3);
          }
          else if(memberData->meta->GetName() == "float")
          {
            *(float*)data = static_cast<float>(luaL_checknumber(L, 3));
          }
          else if(memberData->meta->GetName() == "double")
          {
            *(double*)data = luaL_checknumber(L, 3);
          }
          else if(memberData->meta->GetName() == "char")
          {
            *(char*)data = (char)luaL_checkint(L, 3);
          }
        }
        else if(memberData->meta->GetName() == "string")//std::string special case
        {
          reinterpret_cast<std::string*>(data)->assign(luaL_checkstring(L, 3));
        }
        else // user data generic case
        {
          LuaUserdata* newdata = (LuaUserdata*)lua_touserdata(L, 3);
          //make sure they're the same type before we set them
          if(newdata->GetType() != memberData->meta)
            __debugbreak();

          memcpy(data, newdata->GetData(), (newdata->GetIndirection()) ? sizeof(void*) : memberData->meta->GetSize());
        }
        return 0;
      }
    }
    return -1;
  }

   lua_State * Reflection::LuaManager::GetLua() { return lua_; }

   Script * Reflection::LuaManager::GetCurrentScript() { return (current_scripts_.size()) ? current_scripts_.back() : nullptr; }

   ScriptManager * Reflection::LuaManager::GetScriptManager() { return manager_; }

  int Lua::GenericFunc(lua_State* L)
  {
    int argc = lua_gettop(L);

    int returnValues = 0;
    std::vector<Var*> args;
    
    //get our arguments(argc = 1 means no args)
    if(argc > 1)
    {
      //predeclare our pointers
      LuaUserdata* arg;
      Var* newref;
      //loop through and grab the arguments
      for(int i = 1; i < argc; ++i)
      { 
        //allocate the reference and luadata on the stack
        
        

        newref = (Var*)alloca(sizeof(Var));
        memset(newref, 0, sizeof(Var));
        bool isCharStar = false;
        int argType = lua_type(L, lua_gettop(L));
        switch(argType)
        {
          case LUA_TBOOLEAN:
          {
            // grab boolean value, set our luadata
            bool copyFrom = (lua_toboolean(L, lua_gettop(L)) != 0);
            arg = (LuaUserdata*)alloca(sizeof(LuaUserdata) + sizeof(bool));
            memset(arg, 0, sizeof(LuaUserdata) + sizeof(bool));
            arg->SetObject(FindType(bool), &copyFrom);
            break;
          }
          case LUA_TNUMBER:
          {
            // grab double value, set our luadata
            double copyFrom = lua_tonumber(L, lua_gettop(L));
            arg = (LuaUserdata*)alloca(sizeof(LuaUserdata) + sizeof(double));
            memset(arg, 0, sizeof(LuaUserdata) + sizeof(double));
            arg->SetObject(FindType(double), &copyFrom);
            break;
          }
          case LUA_TSTRING:
          {
            // grab std::string value, set our luadata
            const char* copyFrom = lua_tostring(L, lua_gettop(L));
            arg = (LuaUserdata*)alloca(sizeof(LuaUserdata) + strlen(copyFrom) + 1);
            memset(arg, 0, sizeof(LuaUserdata) + strlen(copyFrom) + 1);
            memcpy(arg->GetData(), copyFrom, strlen(copyFrom) + 1);
            arg->SetType(FindType(char));
            isCharStar = true;
            break;
          }
          case LUA_TUSERDATA:
          {
            // grab luadata, we can just overwrite the luadata pointer for this
            arg = (LuaUserdata*)lua_touserdata(L, lua_gettop(L));
            break;
          }
          case LUA_TNIL:
          {
            __debugbreak();
            //WSError("Lua Error: Passed Nil value to function in lua, can't call.");
            return 0;
          }
          case LUA_TLIGHTUSERDATA:
          {
            __debugbreak();
            //WSError("Lua Error: Passed a pointer?? to function in lua.");
            return 0;
          }
          case LUA_TTABLE:
          {
            __debugbreak();
            //WSError("Lua Error: Passed a table to lua. Use \".value\"");
            return 0;
          }
          default:
            __debugbreak();
            //WSError("Lua Error: Fuck what");
            return 0;
        }

        //set the refvariant and push it back, pop the argument off the stack
        new (newref) Var(arg);
        if(isCharStar)
        {
          newref->SetLevelsOfIndirection(1);
        }
        args.push_back(newref);
        lua_pop(L, 1);
      }
      
      //reverse the order so we may pass the arguments in a manner that makes sense
      std::reverse(args.begin(), args.end());
    }
    else if(!argc) // no args means no method
    {
      __debugbreak();
      //Warn("Lua Error: Woah what. There's no function to call. Did you accidentally type a dot when you needed a colon?");
      return 0;
    }

    std::string methodName = find_function_name(L);
    Var self;
    //get our base object
    void* object = nullptr;
    Metadata* selfType = nullptr;
    //make sure the top is userdata and not a table
    switch(lua_type(L, lua_gettop(L)))
    {
      case LUA_TTABLE:
        luaL_error(L, ("No \"this\" used on call to " + methodName + "\n").c_str());
      case LUA_TUSERDATA:
      {
        //grab userdata
        LuaUserdata* luadata = (LuaUserdata*)lua_touserdata(L, lua_gettop(L));

        object = luadata->GetData();

        self.SetLevelsOfIndirection(luadata->GetIndirection());
        self.SetData(object);
        self.SetType(luadata->GetType());
        selfType = luadata->GetType();
      }
    }

    lua_pop(L, 1);

    if(selfType)
    {
      auto it = selfType->GetMethods().find(methodName);
      if(it != selfType->GetMethods().end())
      {
        if(object)
        {
          Var returnValue;
          Metadata* ret_type = selfType->GetMethod(methodName)->return_type_;
          LuaUserdata* newdata = nullptr;//return type
          if(ret_type)
          {
            //allocate our return type and setup our retval variant
            newdata = (LuaUserdata*)lua_newuserdata(L, sizeof(LuaUserdata) + ret_type->GetSize());
            memset(newdata, 0, sizeof(LuaUserdata) + ret_type->GetSize());
            
            newdata->SetType(ret_type);
            returnValue.SetType(ret_type);
            returnValue.SetData(newdata->GetData());

            returnValue.Validate();
          }
          

          //call the function, depositing the return value into the lua userdata
          Function& func = *(selfType->GetMethod(methodName));
          func(&self, &returnValue, args);
          //data versioning. need instant writes to references on modification. need to get any modified data when we use it.
          //generate a new hash for "thisObject"
          

          if(ret_type) // if we had a return type
          {
            bool isValid = true;
            if(returnValue.GetIndirection())
            {
              void* ptr = *((char**)returnValue.GetData());
              if(!ptr)
              {
                isValid = false; 
              }
            }
            if(isValid)
            {
              //more deprecated
              /*if(ret_type->IsBuiltInType()) // if it's a built in type, reset the return type accordingly
              {
                if(ret_type->GetEnumConstants().size())
                {
                  ret_type = FindType(LuaInt);
                }
                if(ret_type->GetName() == "bool")
                {
                  ret_type = FindType(LuaBool);
                }
                else if(ret_type->GetName() == "long")
                {
                  ret_type = FindType(LuaLong);
                }
                else if(ret_type->GetName() == "int")
                {
                  ret_type = FindType(LuaInt);
                }
                else if(ret_type->GetName() == "unsigned")
                {
                  ret_type = FindType(LuaUnsigned);
                }
                else if(ret_type->GetName() == "float")
                {
                  ret_type = FindType(LuaFloat);
                }
                else if(ret_type->GetName() == "double")
                {
                  ret_type = FindType(LuaDouble);
                }
                else if(ret_type->GetName() == "char")
                {
                  if(returnValue.GetIndirection() == 1)
                  {
                    ret_type = FindType(LuaString);
                  }
                  else
                  {
                    ret_type = FindType(LuaString);
                  }
                }
              }*/
              // initialize our lua userdata
              newdata->SetType(ret_type);
              newdata->SetIndirection(returnValue.GetIndirection());
              //set the metatable for userdata
              luaL_getmetatable(L, ret_type->GetName().c_str());
              lua_setmetatable(L, -2);

              returnValues = 1;

            }
            else
            {
              lua_pop(L, 1);
              lua_pushnil(L);
            }
          }
        }
      }
      else
      {
        std::string errorString = "Calling non-existant member function: " + methodName + " on type: " + selfType->GetName();
        return luaL_error(L, errorString.c_str());
      }
    }
    else
    {
      std::string errorString = "Calling non-existant member function: " + methodName + " on non-existant type";
      return luaL_error(L, errorString.c_str());
    }
    //satisfies warning
    return returnValues;
  }

  static int test(lua_State*)
  {
    return 0;
  }

  static int addStuffIGuess(lua_State* state)
  {
    return 0;
  }

  static int subtractStuffIGuess(lua_State* state)
  {
    return 0;
  }

  bool LuaManager::Load()
  {
    manager_->LoadScripts();
    return true;
  }

  LuaManager::LuaManager()
  {
    //meta function
    MetaManager::DefineConversions();

    //open lua libs
    lua_ = luaL_newstate();
    luaL_openlibs(lua_);

    //for redirecting print
    lua_getglobal(lua_, "_G");
    luaL_register(lua_, NULL, printlib);
    lua_pop(lua_, 1);
    LuaMemberdata memberData;
    auto& metamap = MetaManager::getMetas();
    for(auto& mit : metamap)
    {
      Metadata* metaData = mit.second;
      auto& luaMethodVector = metaData->GetLuaMethods();
      auto& luaMetaMethodVector = metaData->GetLuaMetaMethods();

      auto& getterVector = metaData->GetLuaGetters();
      auto& setterVector = metaData->GetLuaSetters();


      auto& memberMap = metaData->GetMembers();
      auto& methodMap = metaData->GetMethods();
      
      auto& ctors = metaData->GetConstructors();


      luaMethodVector.reserve(methodMap.size() + ctors.size() + 2);
      luaMetaMethodVector.reserve(10);

      getterVector.reserve(memberMap.size() + 1);
      setterVector.reserve(memberMap.size() + 1);

      for(auto it = memberMap.begin(); it != memberMap.end(); ++it)
      {
        
        Member& member = it->second;
        memberData.name = it->first.c_str();
        memberData.meta = member.GetType();
        memberData.type = LuaMemberdata::Get;
        memberData.offset = member.GetOffset();
        memberData.indirection = member.GetIndirection();

        getterVector.push_back(memberData);
        memberData.type = LuaMemberdata::Set;
        setterVector.push_back(memberData);
      }

      memberData.name = "this";
      memberData.meta = metaData;
      memberData.type = LuaMemberdata::Get;
      memberData.offset = 0;
      memberData.indirection = 0;
      getterVector.push_back(memberData);
      memberData.type = LuaMemberdata::Set;
      setterVector.push_back(memberData);

      for(auto it = ctors.begin(); it != ctors.end(); ++it)
      {
        luaMethodVector.push_back({it->first.c_str(), Lua::Allocate});
      }
      luaMethodVector.push_back({"new", Lua::Allocate});

      for(auto it = methodMap.begin(); it != methodMap.end(); ++it)
      {
        luaMethodVector.push_back({it->first.c_str(), Lua::GenericFunc});
      }
      luaMethodVector.push_back({nullptr, nullptr});

      if (metaData->IsBuiltInType())
      {

      }
      luaMetaMethodVector.push_back({"__gc", test});
      luaMetaMethodVector.push_back({nullptr, nullptr});

      luaL_openlib(lua_, 
                   metaData->GetName().c_str(), 
                   &luaMethodVector[0], 
                   0);
      int methods = lua_gettop(lua_);

      luaL_newmetatable(lua_, 
                        metaData->GetName().c_str());

      luaL_openlib(lua_, 
                   0, 
                   &luaMetaMethodVector[0], 
                   0);

      int metatable = lua_gettop(lua_);

      lua_pushliteral(lua_, "__metatable");
      lua_pushvalue(lua_, methods);
      lua_rawset(lua_, metatable);

      lua_pushliteral(lua_, "__index");
      lua_pushvalue(lua_, metatable);
      Lua::add(lua_, getterVector);
      lua_pushvalue(lua_, methods);
      lua_pushcclosure(lua_, Lua::index_handler, 2);
      lua_rawset(lua_, metatable);     /* metatable.__index = index_handler */

      lua_pushliteral(lua_, "__newindex");
      lua_newtable(lua_);              /* table for members you can set */
      Lua::add(lua_, setterVector);     /* fill with setters */
      lua_pushcclosure(lua_, Lua::newindex_handler, 1);
      lua_rawset(lua_, metatable);     /* metatable.__newindex = newindex_handler */

      lua_pop(lua_, 1);                /* drop metatable */
    }

    lua_register(lua_, "GetArgs", reinterpret_cast<lua_CFunction>(&Lua::GetArgs));
    lua_register(lua_, "RefCast", reinterpret_cast<lua_CFunction>(&Lua::RefCast));
    manager_ = new ScriptManager();
    current_script_ = nullptr;

  }

  LuaManager::~LuaManager()
  {  
    lua_close(lua_);
    delete manager_;
  }

  Script* LuaManager::AddScript(const std::string& filename)
  {
    return manager_->MakeScript(filename);
  }

  void LuaManager::RunScript(const std::string& script)
  {
    //std::cout << lua_gettop(lua_) << std::endl;
    //manager_->GetScript(script)->Run();
  }

  void LuaManager::set_current_script(Script * s)
  {
    current_scripts_.push_back(s);
  }

  void LuaManager::end_script()
  {
    current_scripts_.pop_back();
  }
}

RegisterType(Reflection, LuaManager)

}
