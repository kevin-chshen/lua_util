#include "test_lua_base.h"

#include <stdio.h>
#include <errno.h>
#include <filesystem>
#include <iostream>
#include <string.h>


int error_handler(lua_State* L)
{
    auto ret = lua_tostring(L, -1);
    printf("error_handler msg: %s\n", ret);
    return 0;
}

void test_lua_base::call(lua_State* L)
{
    auto ret = luaL_dofile(L, "script.lua");
    if (ret != LUA_OK)
    {
        printf("script load error!! ret %d\n", ret);
        return;
    }

    lua_getglobal(L, "test_call");
    lua_stack_depth(L);
    lua_pushinteger(L, 1);
    lua_stack_depth(L);
    lua_pushinteger(L, 2);
    lua_stack_depth(L);

    lua_call(L, 2, 1);
    lua_stack_depth(L);
    size_t num = lua_tointeger(L, -1);
    lua_stack_depth(L);
    printf("call test_call result : %zd\n", num);

    lua_pop(L, 1);
    lua_stack_depth(L);

}

void test_lua_base::pcall(lua_State* L)
{
    auto ret = luaL_dofile(L, "script.lua");
    if (ret != LUA_OK)
    {
        printf("script load error!! ret %d\n", ret);
        return;
    }

    lua_getglobal(L, "test_call");
    lua_stack_depth(L);
    lua_pushinteger(L, 1);
    lua_stack_depth(L);
    lua_pushinteger(L, 2);
    lua_stack_depth(L);

    /*lua_pushcfunction(L, error_handler);
    lua_stack_depth(L);*/
    auto call = lua_pcall(L, 2, 1, 0);
    if (call != LUA_OK)
    {
        printf("call function error!! ret : %d \n", call);
        lua_stack_depth(L);
        //error_handler(L);
        //lua_pop(L, 1);  // 弹出错误信息
        lua_stack_trace(L);
        //auto depth = lua_gettop(L);
        //lua_pop(L, depth);
        return;
    }
    size_t num = lua_tointeger(L, -1);
    lua_stack_depth(L);
    printf("call test_call result : %zd\n", num);

    lua_pop(L, 1);
    lua_stack_depth(L);
}

void test_lua_base::test(lua_State* L)
{

}

void test_lua_base::lua_stack_depth(lua_State* L)
{
    auto depth = lua_gettop(L);
    printf("lua cur depth %d, %s \n", depth, lua_tostring(L, depth));
}

void test_lua_base::lua_stack_trace(lua_State* L)
{
    int top = lua_gettop(L);  // 获取堆栈顶部索引

    lua_Debug ar;
    int level = 0;
    while(lua_getstack(L, level, &ar))
    {
        level++;
    }
    if (level > 0)
    {
        printf("Lua堆栈跟踪信息:\n");
        lua_getstack(L, level, &ar);
        lua_getinfo(L, "nSl", &ar);  // 获取调用信息
        printf("函数 %s 在文件 %s 的第 %d 行\n", ar.name, ar.source, ar.currentline);
    }
    else
    {
        auto depth = lua_gettop(L);
        printf("%s \n", lua_tostring(L, depth));
    }

}

void test_lua_base::new_table(lua_State* L)
{
    lua_newtable(L);  // 创建一个新的表

   // 添加键值对
    lua_pushstring(L, "key1");
    lua_stack_depth(L);
    lua_pushstring(L, "value1");
    lua_copy(L, 2, -1);
    //lua_settop(L, 2);
    lua_stack_depth(L);
    lua_settable(L, -3);
    lua_stack_depth(L);

    lua_pushstring(L, "key2");
    lua_stack_depth(L);
    lua_pushinteger(L, 42);
    lua_stack_depth(L);
    lua_settable(L, -3);
    lua_stack_depth(L);

    // 在全局环境中设置变量 "myTable" 为新创建的表
    lua_setglobal(L, "myTable");
    lua_stack_depth(L);
}

void test_lua_base::stack_dump(lua_State* L)
{
    int top = lua_gettop(L);
    for (int i = 1; i <= top; ++i)
    {
        int t = lua_type(L, i);
        switch (t)
        {
        case LUA_TSTRING:
        {
            printf("'%s'", lua_tostring(L, i));
            break;
        }
        case LUA_TBOOLEAN:
        {
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
        }
        case LUA_TNUMBER:
        {
            if (lua_isinteger(L, i))
            {
                printf("%lld", lua_tointeger(L, i));
            }
            else
            {
                printf("%g", lua_tonumber(L, i));
            }
            break;
        }
        default:
            printf("%s", lua_typename(L, t));
            break;
        }
        printf("\t");
    }
    printf("\n");
}

void test_lua_base::other(lua_State* L)
{
    lua_pushboolean(L, 1);
    lua_pushnumber(L, 10);
    lua_pushnil(L);
    lua_pushstring(L, "hello");

    test_lua_base::stack_dump(L);

    lua_pushvalue(L, -1);
    test_lua_base::stack_dump(L);

    lua_pushstring(L, "world!");
    lua_replace(L, 3);
    test_lua_base::stack_dump(L);

    lua_settop(L, 6);
    test_lua_base::stack_dump(L);

    lua_rotate(L, 2, 2);
    test_lua_base::stack_dump(L);

    lua_remove(L, -3);
    test_lua_base::stack_dump(L);

    lua_settop(L, -3);
    test_lua_base::stack_dump(L);
}

int test_lua_base::getglobalint(lua_State* L, const char* var)
{
    int isnum, result;
    lua_getglobal(L, var);
    result = (int)lua_tointegerx(L, -1, &isnum);
    if (!isnum)
    {
        printf("%s var: \n", var);
    }
    printf("%d isnum: \n", isnum);
    lua_pop(L, 1);
    return result;
}

void test_lua_base::load(lua_State* L, const char* fname)
{
    if (0 != luaL_dofile(L, fname))
    {
        printf("%s \n", lua_tostring(L, -1));
    }

    //int w = getglobalint(L, "width");
    //int h = getglobalint(L, "heigh");
    //printf("w: %d h: %d \n", w, h);
}

void test_lua_base::getbackground(lua_State* L)
{
    lua_getglobal(L, "background");
    if (!lua_istable(L, -1))
        error(L, "background is not table", "");

    int red = getcolorfield(L, "red");
    printf("red is %d \n", red);
    int blue = getcolorfield(L, "blue");
    printf("blue is %d \n", blue);
    int green = getcolorfield(L, "green");
    printf("green is %d \n", green);
}

int test_lua_base::getcolorfield(lua_State* L, const char* key)
{
    int result, isnum;
    /*lua_pushstring(L, key);
    lua_gettable(L, -2);*/
    // 替换为
    lua_getfield(L, -1, key);
    result = (int)(lua_tonumberx(L, -1, &isnum) * MAX_COLOR);
    if (!isnum)
    {
        error(L, "invalid component %s in color", key);
    }
    lua_pop(L, 1);
    return result;
}

void test_lua_base::error(lua_State* L, const std::string& fmt, const char* err)
{
    printf(fmt.c_str(), err);
    lua_error(L);
}

void test_lua_base::cgrb(lua_State* L)
{
    color_table colortable[] = {
        {"WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR},
        {"RED", MAX_COLOR, 0, 0},
        {"GREEN", 0, MAX_COLOR, 0},
        {"BLUE", 0, 0, MAX_COLOR},
        {nullptr, 0, 0, 0}
    };

    int i = 0;
    while (colortable[i].name != nullptr)
    {
        csetcolor(L, &colortable[i]);
        i++;
    }

    lua_getglobal(L, "background");

    int red = 0;
    int green = 0;
    int blue = 0;

    if (lua_isstring(L, -1))
    {
        const char* colorname = lua_tostring(L, -1);
        int i = 0;
        for (; colortable[i].name != nullptr; ++i)
        {
            if (std::strcmp(colorname, colortable[i].name) == 0)
                break;
        }
        if (colortable[i].name == nullptr)
        {
            error(L, "invalid color name %s \n", colorname);
        }
        else
        {
            red = colortable[i].red;
            green = colortable[i].green;
            blue = colortable[i].blue;
        }
    }
    else if (lua_istable(L, -1))
    {
        red = getcolorfield(L, "red");
        green = getcolorfield(L, "green");
        blue = getcolorfield(L, "blue");
    }
    else
    {
        lua_getglobal(L, "RED");
        if (lua_istable(L, -1))
        {
            red = getcolorfield(L, "red");
            green = getcolorfield(L, "green");
            blue = getcolorfield(L, "blue");
            printf("red %d, green %d blue %d \n", red, green, blue);
        }
        else
        {
            error(L, "invalid value form background!!", "");
        }
    }
}

void test_lua_base::csetcolor(lua_State* L, color_table* ct)
{
    lua_newtable(L);
    csetcolorfield(L, "red", ct->red);
    csetcolorfield(L, "green", ct->green);
    csetcolorfield(L, "blue", ct->blue);
    lua_setglobal(L, ct->name);
}

void test_lua_base::csetcolorfield(lua_State* L, const char* index, int value)
{
    /*lua_pushstring(L, index);
    lua_pushnumber(L, (double)value / MAX_COLOR);
    lua_settable(L, -3);*/
    // 优化后
    lua_pushnumber(L, (double)value / MAX_COLOR);
    lua_setfield(L, -2, index);
}

void test_lua_base::f_call(lua_State* L)
{
    lua_getglobal(L, "test_lua");
    if (0 != lua_pcall(L, 0, 0, 0))
    {
        //error(L, "invaild function", "");
        lua_stack_trace(L);
    }
}

void test_lua_base::call_va(lua_State* L, const char* func, const char* sig, ...)
{
    va_list vl;
    int narg = 2;
    int nres = 1;

    va_start(vl, sig);
    lua_getglobal(L, func);

    narg = va_narg(L, vl, sig);
    nres = (int)strlen(sig);

    if (lua_pcall(L, narg, nres, 0) != 0)
    {
        auto rstr = lua_tostring(L, -1);
        error(L, "error calling %s  \n", func);
    }

    nres = va_nres(L, vl, sig);

    va_end(vl);
}

int test_lua_base::va_narg(lua_State* L, va_list& vl, const char*& sig)
{
    int narg = 0;
    for (; *sig; narg++)
    {
        lua_checkstack(L, 1);

        switch (*sig++)
        {
        case 'd':
            lua_pushnumber(L, va_arg(vl, double));
            break;
        case 'i':
            lua_pushinteger(L, va_arg(vl, int));
            break;
        case 's':
            lua_pushstring(L, va_arg(vl, char*));
            break;
        case '>':
            return narg;
            break;
        default:
        {
            error(L, "invalid option %c \n", (sig - 1));
        }break;
        }
    }
    return narg;
}

int test_lua_base::va_nres(lua_State* L, va_list& vl, const char* sig)
{
    int nres = 0;
    while (*sig)
    {
        switch (*sig++)
        {
        case 'd':
        {
            int isnum = 0;
            double n = lua_tonumberx(L, nres, &isnum);
            if (!isnum)
            {
                error(L, "wrong result type", "");
            }
            *va_arg(vl, double*) = n;
        }break;
        case 'i':
        {
            int isnum = 0;
            int n = (int)lua_tointegerx(L, nres, &isnum);
            if (!isnum)
            {
                error(L, "wrong result type", "");
            }
            *va_arg(vl, int*) = n;
        }break;
        case 's':
        {
            const char* s = lua_tostring(L, nres);
            if (nullptr == s)
            {
                error(L, "wrong result type", "");
            }
            *va_arg(vl, const char**) = s;
        }break;
        default:
            error(L, "invalid option res %c ", sig - 1);
            break;
        }
        nres++;
    }

    return 0;
}

int test_lua_base::l_dir(lua_State* L)
{
    if (!lua_isstring(L, -1))
    {
        std::cout << "dir path is not string!!" << std::endl;
        return 1;
    }
    const char* dir = lua_tostring(L, 1);
    std::filesystem::path dirPath(dir);

    if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath))
    {
        lua_newtable(L);
        int i = 1;
        for (const auto& entry : std::filesystem::directory_iterator(dirPath))
        {
            std::cout << entry.path().filename().string() << std::endl;
            lua_pushinteger(L, i);
            lua_pushstring(L, entry.path().filename().string().c_str());
            lua_settable(L, -3);
        }
    }
    else
    {
        lua_pushnil(L);
        lua_pushstring(L, "");
        std::cout << "目录不存在或者不是一个有效的目录！" << std::endl;
        return 2;
    }
    return 1;
}

void test_lua_base::l_function(lua_State* L, lua_CFunction cfunc, const std::string& name)
{
    lua_pushcfunction(L, cfunc);
    lua_setglobal(L, name.c_str());
}

int test_lua_base::finishpcall(lua_State* L, int status, intptr_t ctx)
{
    std::cout << "finishcall \n";
    (void)ctx; // 未使用参数
    status = (status != LUA_OK && status != LUA_YIELD);
    //lua_pushboolean(L, status == 0); // 状态
    lua_pushinteger(L, status); // 状态
    lua_insert(L, 1);    // 状态是第一个结果
    return lua_gettop(L); // 返回状态和所有结果
}

int test_lua_base::luaB_pcall(lua_State* L)
{
    int status;
    luaL_checkany(L, 1);
    std::cout << "lua_pcallk before \n";
    status = lua_pcallk(L, lua_gettop(L) - 1, LUA_MULTRET, 0, 0, finishpcall);
    std::cout << "lua_pcallk after \n";
    return finishpcall(L, status, 0);
}

int test_lua_base::luaA_pcall(lua_State* L)
{
    int status;
    luaL_checkany(L, 1);
    std::cout << "lua_pcall before \n";
    status = lua_pcall(L, lua_gettop(L) - 1, LUA_MULTRET, 0);
    std::cout << "lua_pcall after \n";
    lua_pushinteger(L, status);
    lua_stack_depth(L);
    lua_insert(L, 1);
    lua_stack_depth(L);
    return lua_gettop(L);
}

int test_lua_base::luaopen_mylib(lua_State* L)
{
    static const struct luaL_Reg mylib[] = {
        {"dir", l_dir},
        {"pcall_A", luaA_pcall},
        {"pcall_B", luaB_pcall},
        {NULL, NULL}
    };

    luaL_newlib(L, mylib);

    return 1;
}

int test_lua_base::l_map(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    luaL_checktype(L, 2, LUA_TFUNCTION);
    lua_stack_depth(L);
    auto n = luaL_len(L, 1);
    lua_stack_depth(L);
    for (int i = 1; i <= n; ++i)
    {
        lua_pushvalue(L, 2);
        lua_stack_depth(L);
        auto ret = lua_geti(L, 1, i);
        std::cout << "get i ret : " << i << ", " << ret << "\n";
        lua_stack_depth(L);
        lua_call(L, 1, 1);
        lua_stack_depth(L);
        lua_seti(L, 1, i);
        lua_stack_depth(L);
    }

    return 0;
}

int test_lua_base::l_split(lua_State* L)
{
    const char* s = luaL_checkstring(L, 1);
    const char* sep = luaL_checkstring(L, 2);
    const char* e;
    int i = 1;
    
    lua_newtable(L);
    while ((e = strchr(s, *sep)) != nullptr)
    {
        lua_stack_depth(L);
        lua_pushlstring(L, s, e - s);
        lua_stack_depth(L);
        lua_rawseti(L, -2, i++);
        s = e + 1;
    }

    lua_stack_depth(L);
    lua_pushstring(L, s);
    lua_stack_depth(L);
    lua_rawseti(L, -2, i);
    lua_stack_depth(L);

    return 1;
}

int test_lua_base::l_upper(lua_State* L)
{
    size_t l;
    luaL_Buffer b;
    const char* s = luaL_checklstring(L, 1, &l);
    char* p = luaL_buffinitsize(L, &b, l);
    lua_stack_depth(L);
    for (int i = 0; i < l; ++i)
    {
        p[i] = toupper(unsigned char(s[i]));
    }
    luaL_pushresultsize(&b, l);
    lua_stack_depth(L);
    return 1;
}

int test_lua_base::l_tconcat(lua_State* L)
{
    luaL_Buffer b;

    luaL_checktype(L, 1, LUA_TTABLE);
    auto n = luaL_len(L, 1);
    luaL_buffinit(L, &b);

    for (int i = 1; i < n; ++i)
    {
        //std::cout << "i " << i << "\n";
        lua_geti(L, 1, i);
        /*lua_pushinteger(L, i);
        lua_gettable(L, -2);*/
        //lua_stack_depth(L);
        luaL_addvalue(&b);
    }
    luaL_pushresult(&b);

    return 1;
}

void test_lua_base::l_register(lua_State* L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, "Key");
    //lua_stack_depth(L);
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    std::cout << "ref " << ref << "\n";
    //lua_stack_depth(L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    //lua_stack_depth(L);
    luaL_unref(L, LUA_REGISTRYINDEX, ref);
    //lua_stack_depth(L);
    luaL_unref(L, LUA_REGISTRYINDEX, LUA_REFNIL);
    //lua_stack_depth(L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_REFNIL);
    //lua_stack_depth(L);

    static char Key = 'k';
    const char myStr[] = "my_str";

    // 保存字符串
    //lua_pushlightuserdata(L, (void*)&Key);
    //lua_pushstring(L, myStr);
    //lua_settable(L, LUA_REGISTRYINDEX);    // register[&Key] = myStr
    lua_pushstring(L, myStr);
    lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)&Key);

    // 获取字符串
    /*lua_pushlightuserdata(L, (void*)&Key);
    lua_gettable(L, LUA_REGISTRYINDEX);*/
    lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)&Key);
    auto myStr2 = lua_tostring(L, -1);
    std::cout << "myStr " << myStr2 << "\n";
}

int test_lua_base::counter(lua_State* L)
{
    auto up = lua_upvalueindex(1);
    auto val = lua_tointeger(L, up);
    lua_pushinteger(L, ++val);
    lua_copy(L, -1, lua_upvalueindex(1));
    return 1;
}

int test_lua_base::new_counter(lua_State* L)
{
    lua_pushinteger(L, 1);
    lua_pushcclosure(L, &counter, 1);
    /* 相当于
    local function xcounter()
        local n = 1
        local function counter(n)
            return 1+n
        end
        return counter(n)
    end
    */
    return 1;
}
