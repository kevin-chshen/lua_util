#pragma once
#include <lua.hpp>

namespace example
{
    int t_tuple(lua_State* L);

    int t_new(lua_State* L);

    int luaopen_tuple(lua_State* L);
}