#ifndef LUA_BINDINGS_H
#define LUA_BINDINGS_H

#ifdef _WIN32
extern "C"{
	#include "../Lua542/include/lua.h"
	#include "../Lua542/include/lauxlib.h"
	#include "../Lua542/include/lualib.h"
}
#else
	#include "../third_party/luajit/src/lua.hpp"
#endif

int getIntField(lua_State* L, const char* key)
{
    lua_pushstring(L, key);
    lua_gettable(L, -2);  // get table[key]
 
    int result = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);  // remove number from stack
    return result;
}
 
std::string getStringField(lua_State* L, const char* key)
{
    lua_pushstring(L, key);
    lua_gettable(L, -2);  // get table[key]
 
    std::string result = lua_tostring(L, -1);
    lua_pop(L, 1);  // remove string from stack
    return result;
}

#endif