#pragma once

#include <lua.hpp>
#include <string>

#include "t_tuple.h"

#define MAX_COLOR 255

class test_lua_base
{
    struct color_table
    {
        const char* name;
        unsigned char red, green, blue;
    };

    typedef int (*lua_CFunction)(lua_State* L);

public:
    static void run()
    {
        test_lua_base o;
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);

        //o.pcall(L);
        /*o.new_table(L);
        o.pcall(L);*/
        //o.other(L);
        o.load(L, "script.lua");
        //o.getbackground(L);
        //o.cgrb(L);
        //o.l_function(L, l_dir, "read_dir");
        //o.l_function(L, luaA_pcall, "corutine_pcall");
        //o.luaopen_mylib(L);
        //o.l_function(L, l_split, "l_split");
        //o.l_function(L, l_upper, "l_upper");
        //o.l_function(L, l_tconcat, "l_tconcat");
        //o.l_register(L);
        //o.l_function(L, new_counter, "new_counter");
        example::luaopen_tuple(L);
        o.f_call(L);
        /*int z = 1;
        o.call_va(L, "f", "dd>d", 3, 2, &z);
        printf("f %d \n", z);*/
        

        lua_close(L);
    }

    void call(lua_State* L);

    void pcall(lua_State* L);

    void test(lua_State* L);

    static void lua_stack_depth(lua_State* L);

    void lua_stack_trace(lua_State* L);

    void new_table(lua_State* L);

    static void stack_dump(lua_State* L);

    void other(lua_State* L);

    int getglobalint(lua_State* L, const char* var);

    void load(lua_State* L, const char* fname);

    void getbackground(lua_State* L);

    int getcolorfield(lua_State* L, const char* key);

    void error(lua_State* L, const std::string& fmt, const char* err);

    void cgrb(lua_State* L);

    void csetcolor(lua_State* L, struct color_table* ct);

    void csetcolorfield(lua_State* L, const char* index, int value);

    void f_call(lua_State* L);

    void call_va(lua_State* L, const char* func, const char* sig, ...);

    int va_narg(lua_State* L, va_list& vl, const char*& sig);

    int va_nres(lua_State* L, va_list& vl, const char* sig);

    static int l_dir(lua_State* L);

    void l_function(lua_State* L, lua_CFunction cfunc, const std::string& name);

    static int finishpcall(lua_State* L, int status, intptr_t ctx);

    static int luaB_pcall(lua_State* L);

    static int luaA_pcall(lua_State* L);

    static int luaopen_mylib(lua_State* L);

    static int l_map(lua_State* L);

    static int l_split(lua_State* L);

    static int l_upper(lua_State* L);

    static int l_tconcat(lua_State* L);

    void l_register(lua_State* L);

    static int counter(lua_State* L);

    static int new_counter(lua_State* L);
};

