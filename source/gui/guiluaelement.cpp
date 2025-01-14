#include <libgui.h>
#include <unistd.h>
#include <limits.h>
#include <lua.hpp>
#include "guiluaelement.h"
#include "luasupport.h"
#include "system.h"
#include "debug.h"

GuiLuaElement::GuiLuaElement() {
    L = NULL;
    basePath[0] = '/';
    basePath[1] = '\0';
    hasToLoadScript = false;
}

GuiLuaElement::~GuiLuaElement() {
    lua_close(L);
}

void GuiLuaElement::onActiveEvent() {
    if (hasToLoadScript) {
        hasToLoadScript = false;

        char oldPath[PATH_MAX];
        getcwd(oldPath, PATH_MAX);
        if (basePath[0])
            chdir(basePath);


        luaSetGuiParentWindow((GuiWindow*)this->parentElement);

        if (int err = luaL_dofile(L, scriptPath.c_str())) {
            systemError("LUA error! loadScript", "Error %d %s", err, lua_tostring(L, -1));
        }
        //Call init in LUA if it exists
        lua_getglobal(L, "init");
        if (int err = lua_pcall(L, 0, 0, 0)) {
            if (err != LUA_ERRRUN)
                systemError("LUA error! init()", "Error %d %s", err, lua_tostring(L, -1));
        }

        chdir(oldPath);
    }
}

void GuiLuaElement::initLUA() {
    if (L != NULL) {
        lua_close(L);
    }

    //Init LUA
    L = luaL_newstate();
    luaL_openlibs(L);

    //Register custom libraries
    luaRegisterCustomLibs(L);
}

void GuiLuaElement::setPath(const char* path) {
    strcpy(basePath, path);
}

void GuiLuaElement::loadScript(const char* script) {
    initLUA();

    char oldPath[PATH_MAX];
    getcwd(oldPath, PATH_MAX);
    if (basePath[0])
        chdir(basePath);


    luaSetGuiParentWindow((GuiWindow*)this->parentElement);

    if (int err = luaL_dostring(L, script)) {
        systemError("LUA error! loadScript", "Error %d %s", err, lua_tostring(L, -1));
    }
    //Call init in LUA if it exists
    lua_getglobal(L, "init");
    if (int err = lua_pcall(L, 0, 0, 0)) {
        if (err != LUA_ERRRUN)
            systemError("LUA error! init()", "Error %d %s", err, lua_tostring(L, -1));
    }

    chdir(oldPath);
}

void GuiLuaElement::loadScriptFile(const char* script) {
    initLUA();

    hasToLoadScript = true;
    scriptPath = script;
}

void GuiLuaElement::draw(bool onFocus) {
    char oldPath[PATH_MAX];
    getcwd(oldPath, PATH_MAX);
    if (basePath[0])
        chdir(basePath);

    luaSetGuiParentWindow((GuiWindow*)this->parentElement);

    //Call draw function in LUA
    lua_getglobal(L, "draw");
    lua_pushboolean(L, onFocus);
    if (int err = lua_pcall(L, 1, 0, 0)) {
        systemError("LUA error! draw()", "Error %d %s", err, lua_tostring(L, -1));
    }

    chdir(oldPath);
}

void GuiLuaElement::handleInputs(bool onFocus) {
    char oldPath[PATH_MAX];
    getcwd(oldPath, PATH_MAX);
    if (basePath[0])
        chdir(basePath);

    luaSetGuiParentWindow((GuiWindow*)this->parentElement);

    //Call handleInputs function in LUA
    lua_getglobal(L, "handleInputs");
    lua_pushboolean(L, onFocus);
    if (int err = lua_pcall(L, 1, 0, 0)) {
        systemError("LUA error! handleInputs()", "Error %d %s", err, lua_tostring(L, -1));
    }

    chdir(oldPath);
}

Vector2 GuiLuaElement::getDimensions() {
    char oldPath[PATH_MAX];
    getcwd(oldPath, PATH_MAX);
    if (basePath[0])
        chdir(basePath);

    luaSetGuiParentWindow((GuiWindow*)this->parentElement);

    //Call getDimensions function in LUA
    lua_getglobal(L, "getDimensions");
    if (int err = lua_pcall(L, 0, 1, 0)) { //1 result
        //The function triggered some errors
        systemError("LUA error! getDimensions()", "Error %d %s", err, lua_tostring(L, -1));
    }

    lua_rawgeti(L, -1, 1);
    u32 w = lua_tointeger(L, -1);
    lua_pop(L, 1); //pop w
    lua_rawgeti(L, -1, 2);
    u32 h = lua_tointeger(L, -1);
    lua_pop(L, 1); //pop h
    lua_pop(L, 1); //pop returned table
    //u32 w = lua_tointeger(L, -2);
    //u32 h = lua_tointeger(L, -1);
    //lua_pop(L, 2); //Pop returned numbers

    chdir(oldPath);

    return Vector2(w, h);
}

void GuiLuaElement::copyIncomingTable(lua_State* inState) {
    lua_newtable(L);
    lua_pushnil(inState);  //First key
    while (lua_next(inState, -2) != 0) { //Loop through all table keys
        //key at -2 and value at -1
        if (lua_type(inState, -2) == LUA_TBOOLEAN) {
            lua_pushboolean(L, lua_toboolean(inState, -2));
        }
        else if (lua_type(inState, -2) == LUA_TNUMBER) {
            lua_pushnumber(L, lua_tonumber(inState, -2));
        }
        else if (lua_type(inState, -2) == LUA_TSTRING) {
            lua_pushstring(L, lua_tostring(inState, -2));
        }
        else
            systemError("LUA error! copyIncomingTable()", "Unsupported key type");

        if (lua_type(inState, -1) == LUA_TNIL) {
            lua_pushnil(L);
        } else if (lua_type(inState, -1) == LUA_TBOOLEAN) {
            lua_pushboolean(L, lua_toboolean(inState, -1));
        } else if (lua_type(inState, -1) == LUA_TNUMBER) {
            lua_pushnumber(L, lua_tonumber(inState, -1));
        } else if (lua_type(inState, -1) == LUA_TSTRING) {
            lua_pushstring(L, lua_tostring(inState, -1));
        } else if (lua_type(inState, -1) == LUA_TTABLE) {
            copyIncomingTable(inState);
        } else {
            systemError("LUA error! copyIncomingTable()", "Unsupported value type");
        }
        lua_settable(L, -3);

        //Removes 'value'; keeps 'key' for next iteration
        lua_pop(inState, 1);
    }
}

void GuiLuaElement::handleMessage(lua_State* inState) {
    char oldPath[PATH_MAX];
    getcwd(oldPath, PATH_MAX);
    if (basePath[0])
        chdir(basePath);

    luaSetGuiParentWindow((GuiWindow*)this->parentElement);

    //Call handleMessage function in LUA
    lua_getglobal(L, "handleMessage");
    switch (lua_type(inState, 1)) {
        case LUA_TNIL:
            lua_pushnil(L);
        break;

        case LUA_TBOOLEAN:
            lua_pushboolean(L, lua_toboolean(inState, 1));
        break;

        case LUA_TSTRING:
            lua_pushstring(L, lua_tostring(inState, 1));
        break;

        case LUA_TNUMBER:
            lua_pushnumber(L, lua_tonumber(inState, 1));
        break;

        case LUA_TTABLE:
            copyIncomingTable(inState);
        break;

        default:
            systemError("LUA error! handleMessage()", "Error: Unsupported value type");
        break;
    }

    if (int err = lua_pcall(L, 1, 0, 0)) {
        systemError("LUA error! handleMessage()", "Error %d %s", err, lua_tostring(L, -1));
    }

    chdir(oldPath);
}