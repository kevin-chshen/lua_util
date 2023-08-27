#include "t_tuple.h"

namespace example
{
    int t_tuple(lua_State* L)
    {
        lua_Integer op = luaL_optinteger(L, 1, 0);

        if (0 == op)
        {
            int i = 1;
            for (; !lua_isnone(L, lua_upvalueindex(i)); ++i)
                lua_pushvalue(L, lua_upvalueindex(i));
            return i - 1;
        }
        else
        {
            luaL_argcheck(L, 0 < op && op <= 256, 1, "index out of range!!");
            if (lua_isnone(L, lua_upvalueindex(op)))
                return 0;
            lua_pushvalue(L, lua_upvalueindex(op));
            return 1;
        }
        return 0;
    }

    int t_new(lua_State* L)
    {
        int top = lua_gettop(L);
        luaL_argcheck(L, top < 256, top, "too many fields");
        lua_pushcclosure(L, t_tuple, top);
        return 1;
    }

    int luaopen_tuple(lua_State* L)
    {
        static const struct luaL_Reg tuplelib[] =
        {
            {"new", t_new},
            {nullptr, nullptr}
        };
        luaL_newlib(L, tuplelib);
        return 1;
    }
}