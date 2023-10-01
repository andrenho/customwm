#include "theme.hh"

#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <sstream>

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}
#include <sys/stat.h>

#include "exceptions.hh"

Theme::Theme()
    : L_ptr(luaL_newstate(), [](lua_State* LL) { lua_close(LL); }), L(L_ptr.get())
{
    luaL_openlibs(L);
}

void Theme::load(std::string const& theme_name)
{
    auto ofile = find_file(theme_name);
    if (!ofile) {
        fprintf(stderr, "File for theme '%s' was not found.\n", theme_name.c_str());
        exit(EXIT_FAILURE);
    }

    load_theme_file(*ofile);
}

std::optional<std::string> Theme::find_file(std::string const& theme_file)
{
    struct stat buffer {};
    for (const char* path : theme_paths) {
        std::string full_path = std::string(path) + "/" + theme_file + ".customvm_theme";
        if (stat(full_path.c_str(), &buffer) == 0)
            return full_path;
    }

    return {};
}

void Theme::load_theme_file(std::string const& filename)
{
    std::ifstream t(filename);
    std::stringstream sbuffer;
    sbuffer << t.rdbuf();

    std::string buffer = sbuffer.str();
    int r = luaL_loadbuffer(L, buffer.c_str(), buffer.size(), "theme");
    if (r != LUA_OK)
        throw LuaException("Error loading theme file.");

    if (lua_gettop(L) != 1)
        throw LuaException("The theme file doesn't return anything. Make sure the syntax is `return { ... }`.\n");

    lua_call(L, 0, 1);
    if (lua_type(L, -1) != LUA_TTABLE)
        throw LuaException("The theme file doesn't return a table. Make sure the syntax is `return { ... }`.\n");
}

Padding Theme::read_padding(std::string const& prop_name, Window const& w, std::optional<Padding> default_value) const
{
    if (!push_property(prop_name)) {
        if (default_value)
            return *default_value;
        else
            throw PropertyNotFoundException(prop_name);
    }

    call_lua_window_function(w);

    Padding padding;
    switch (lua_type(L, -1)) {
        case LUA_TNUMBER: {
            padding = Padding((int) lua_tonumber(L, -1));
            break;
        }
        case LUA_TTABLE: {
            auto get_idx = [this](int idx) {
                lua_geti(L, -1, idx);
                int n = (int) luaL_checkinteger(L, -1);
                lua_pop(L, 1);
                return n;
            };
            padding.top = get_idx(1);
            padding.bottom = get_idx(2);
            padding.left = get_idx(3);
            padding.right = get_idx(4);
            break;
        }
        default:
            throw LuaException(prop_name + ": expected a number, table or function");
    }

    empty_stack();

    return padding;
}

bool Theme::push_property(std::string const& prop_name) const
{
    std::istringstream iss(prop_name);
    std::string property;

    while (std::getline(iss, property, '.')) {
        int type = lua_getfield(L, -1, property.c_str());
        if (type == LUA_TNIL || (iss.peek() != EOF /* is not last */ && type != LUA_TTABLE)) {
            empty_stack();
            return false;
        }
    }
    return true;
}

void Theme::empty_stack() const
{
    int to_discard = lua_gettop(L) - 1;
    if (to_discard > 0)
        lua_pop(L, to_discard);
}

bool Theme::call_lua_window_function(Window const& window) const
{
    if (lua_type(L, -1) != LUA_TFUNCTION)
        return false;

    push_window(window);
    lua_call(L, 1, 1);
    return true;
}

void Theme::push_window(const Window &window) const
{
    lua_newtable(L);
    // TODO - convert window
}

