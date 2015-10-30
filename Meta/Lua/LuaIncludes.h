#pragma once

#define LUA_COMPAT_APIINTCASTS
#define LUA_COMPAT_MODULE
extern "C" 
{
  #include "Lua/include/lua.h"
  #include "Lua/include/lualib.h"
  #include "Lua/include/lauxlib.h"
}
