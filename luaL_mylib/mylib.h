#pragma once
#include <lua.hpp>

// 示例 C 函数
int myFunction(lua_State* L) {
    int arg1 = luaL_checkinteger(L, 1);
    int arg2 = luaL_checkinteger(L, 2);
    int sum = arg1 + arg2;

    lua_pushinteger(L, sum);
    return 1;
}

// 定义注册表
static const luaL_Reg myLib[] = {
    {"myFunction", myFunction},
    {NULL, NULL}
};

// 注册函数到 Lua 环境
int luaopen_mylib(lua_State* L) {
    luaL_newlib(L, myLib);
    return 1;
}