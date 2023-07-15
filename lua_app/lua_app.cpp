// lua_app.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <lua.hpp>

int main() {
    lua_State* L = luaL_newstate();  // 创建Lua状态机
    luaL_openlibs(L);  // 打开Lua标准库

    lua_newtable(L);  // 创建一个新的表

    // 添加键值对
    lua_pushstring(L, "key1");
    lua_pushstring(L, "value1");
    lua_settable(L, -3);

    lua_pushstring(L, "key2");
    lua_pushinteger(L, 42);
    lua_settable(L, -3);

    // 在全局环境中设置变量 "myTable" 为新创建的表
    lua_setglobal(L, "myTable");

    lua_close(L);  // 关闭Lua状态机

    return 0;
}