//------------------------------------------------------------------------------
//           Name: lua_simple.cpp
//         Author: Kevin Harris (kevin@codesampler.com)
//  Last Modified: 05/11/05
//    Description: This sample demonstrates the bare essentials of 
//                 running an embedded Lua script from a C/C++ application.
//------------------------------------------------------------------------------

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

int main( int argc, char **argv )
{
	// Initialize Lua
	lua_State *pLuaState = lua_open();

	// Load the Lua libraries
//	lua_baselibopen( pLuaState );
	luaL_openlibs(pLuaState);
	// Use Lua to run a script
	luaL_dofile( pLuaState, "../data/test.lua" );

	// That's it... cleanup after Lua
	lua_close( pLuaState );

	return 0;

//    lua_State* L = luaL_newstate();
//
//    luaL_dostring(L, "a = 10 + 5");
//    lua_getglobal(L, "a");
//    int i = lua_tointeger(L, -1);
//    printf("%d\n", i);
//
//    lua_close(L);
}
