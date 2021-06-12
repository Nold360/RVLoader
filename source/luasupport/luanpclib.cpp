#include <libgui.h>
#include <gccore.h>
#include <lua.hpp>
#include "main.h"
#include "luasupport.h"
#include "npc.h"

static int lua_NPC_isConnected(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushboolean(L, NPC::isConnected());

    return 1;
}

static int lua_NPC_getVer(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, NPC::getVer());

    return 1;
}

static int lua_NPC_getLedCount(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, NPC::getLedCount());

    return 1;
}

static int lua_NPC_getLedMode(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 0) {
        return luaL_error(L, "wrong number of arguments");
    }

    lua_pushnumber(L, NPC::getLedMode());

    return 1;
}

static int lua_NPC_setColor(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 4) {
        return luaL_error(L, "wrong number of arguments");
    }

    NPC::setColor(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));

    return 0;
}

static int lua_NPC_setLedMode(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    NPC::setLedMode(luaL_checkinteger(L, 1));

    return 0;
}

static int lua_NPC_setLedCount(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    NPC::setLedCount(luaL_checkinteger(L, 1));

    return 0;
}

static int lua_NPC_setDisplay(lua_State* L) {
    int argc = lua_gettop(L);
    if (argc != 1) {
        return luaL_error(L, "wrong number of arguments");
    }

    NPC::setDisplay(luaL_checkinteger(L, 1));

    return 0;
}

static const luaL_Reg NPC_functions[] = {
    {"isConnected", lua_NPC_isConnected},
    {"getVer", lua_NPC_getVer},
    {"setColor", lua_NPC_setColor},
    {"setLedMode", lua_NPC_setLedMode},
    {"getLedMode", lua_NPC_getLedMode},
    {"setLedCount", lua_NPC_setLedCount},
    {"getLedCount", lua_NPC_getLedCount},
    {"setDisplay", lua_NPC_setDisplay},
    {NULL, NULL}
};

void luaRegisterNPCLib(lua_State* L) {
    lua_newtable(L);
    luaL_setfuncs(L, NPC_functions, 0);
    luaSetTableIntField(L, "LED_COUNT", 0x00);
    luaSetTableIntField(L, "LED_MODE_STATIC", 0x01);
    luaSetTableIntField(L, "LED_MODE_RAINBOW", 0x02);
    luaSetTableIntField(L, "DISPLAY_UP", 0x30);
    luaSetTableIntField(L, "DISPLAY_DOWN", 0x31);
    luaSetTableIntField(L, "DISPLAY_MENU", 0x32);
    luaSetTableIntField(L, "DISPLAY_ENTER", 0x33);
    lua_setglobal(L, "NPC");
}
