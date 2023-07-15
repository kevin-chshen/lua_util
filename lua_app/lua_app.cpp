// lua_app.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <lua.hpp>

int main() {
    lua_State* L = luaL_newstate();  // ����Lua״̬��
    luaL_openlibs(L);  // ��Lua��׼��

    lua_newtable(L);  // ����һ���µı�

    // ��Ӽ�ֵ��
    lua_pushstring(L, "key1");
    lua_pushstring(L, "value1");
    lua_settable(L, -3);

    lua_pushstring(L, "key2");
    lua_pushinteger(L, 42);
    lua_settable(L, -3);

    // ��ȫ�ֻ��������ñ��� "myTable" Ϊ�´����ı�
    lua_setglobal(L, "myTable");

    lua_close(L);  // �ر�Lua״̬��

    return 0;
}