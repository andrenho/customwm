#include "engine.hh"

#include <cstdint>
#include <stdexcept>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "metatables.hh"
#include "luaw.hh"
#include "helper.hh"

Engine::Engine()
        : L(luaL_newstate())
{
    luaL_openlibs(L);

    metatable_root_create(L);

    load_script_from_memory("helper", helper, helper_len);
}

Engine::~Engine()
{
    lua_close(L);
}

void Engine::load_script_from_memory(std::string const& name, uint8_t *data, unsigned int sz)
{
    if (luaL_loadbufferx(L, (const char *) data, sz, name.c_str(), "b") != LUA_OK)
        throw std::runtime_error("error loading " + name + " from memory");

    if (lua_pcall(L, 0, 0, 0) != LUA_OK)
        throw std::runtime_error("error running " + name + " from memory");

    luaw_asserttop(L, 0);
}

void Engine::load_theme_from_memory(const std::string &name, uint8_t *data, unsigned int len)
{
    if (luaL_loadbufferx(L, (const char *) data, len, name.c_str(), "b") != LUA_OK)
        throw std::runtime_error("error loading " + name + " from memory");

    if (lua_pcall(L, 0, 1, 0) != LUA_OK)
        throw std::runtime_error("error running " + name + " from memory");

    lua_setglobal(L, "theme");

    luaw_asserttop(L, 0);
}

void Engine::override_with_theme(std::string const& filename)
{
}

void Engine::setup_root_object(class Root* root)
{
    RootUserData* ud = (RootUserData *) lua_newuserdata(L, sizeof(RootUserData));
    ud->root = root;
    luaL_setmetatable(L, "__root_mt");
    lua_setglobal(L, "root");
    luaw_asserttop(L, 0);
}
