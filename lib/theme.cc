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

//
// THEME LOADING
//

void Theme::load(std::string const& theme_name)
{
    auto ofile = find_file(theme_name);
    if (!ofile) {
        fprintf(stderr, "File for theme '%s' was not found.\n", theme_name.c_str());
        exit(EXIT_FAILURE);
    }

    load_theme_file(*ofile);
}

void Theme::reload_if_modified()
{
    time_t t = theme_file_last_modified();
    if (t > loaded_theme_last_modified_) {
        lua_pop(L, lua_gettop(L));
        load_theme_file(theme_filename_);
        printf("Theme file reloaded.\n");
    }
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

time_t Theme::theme_file_last_modified()
{
    struct stat buffer {};
    if (stat(theme_filename_.c_str(), &buffer) == 0)
        return buffer.st_mtime;
    else
        return 0;
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

    theme_filename_ = filename;
    loaded_theme_last_modified_ = theme_file_last_modified();
}

//
// PUBLIC READING METHODS
//

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
            padding = Padding((int16_t) lua_tonumber(L, -1));
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
            padding.left = get_idx(2);
            padding.bottom = get_idx(3);
            padding.right = get_idx(4);
            break;
        }
        default:
            throw LuaException(prop_name + ": expected a number, table or function");
    }

    empty_stack();

    return padding;
}

WindowStartingPos Theme::read_starting_pos(const std::string &prop_name, const Window &w) const
{
    if (!push_property(prop_name))
        throw PropertyNotFoundException(prop_name);

    call_lua_window_function(w);

    WindowStartingPos window_starting_pos {};
    switch (lua_type(L, -1)) {
        case LUA_TSTRING: {
            std::string s(lua_tostring(L, -1));
            if (s == "cascade")
                window_starting_pos.starting_pos = WindowStartingPos::Cascade;
            else if (s == "center")
                window_starting_pos.starting_pos = WindowStartingPos::Center;
            else if (s == "random")
                window_starting_pos.starting_pos = WindowStartingPos::Random;
            else if (s == "maximized")
                window_starting_pos.starting_pos = WindowStartingPos::Maximized;
            else if (s == "requested")
                window_starting_pos.starting_pos = WindowStartingPos::Requested;
            else
                throw LuaException("Invalid starting position '" + s + "'");
            break;
        }
        case LUA_TTABLE:
            window_starting_pos = {
                    .starting_pos = WindowStartingPos::Custom,
                    .point = to_point(-1),
            };
            break;
        default:
            throw LuaException(prop_name + ": expected a string, table or function");
    }

    empty_stack();
    return window_starting_pos;
}


void Theme::call_with_window_and_brush(const std::string &prop_name, Window &w)
{
    if (!push_property(prop_name))
        throw PropertyNotFoundException(prop_name);

    if (lua_type(L, -1) != LUA_TFUNCTION)
        throw LuaException("Property " + prop_name + " is not a function.");

    push_window(w, true);
    lua_call(L, 1, 0);
}


//
// LUA STACK MANAGEMENT
//

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

    push_window(window, false);
    lua_call(L, 1, 1);
    return true;
}

void Theme::push_window(const Window &window, bool include_brush) const
{
    lua_newtable(L);
    lua_pushinteger(L, window.w); lua_setfield(L, -2, "w");
    lua_pushinteger(L, window.h); lua_setfield(L, -2, "h");

    if (include_brush) {
        push_brush(*window.brush);
        lua_setfield(L, -2, "brush");
    }
}

void Theme::push_brush(IBrush& brush) const
{
    lua_newtable(L);

    lua_pushcfunction(L, [](lua_State* l) { return 0; });
}

Point Theme::to_point(int i) const
{
    auto get_idx = [this, i](int idx) {
        lua_geti(L, i, idx);
        int n = (int) luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        return n;
    };
    int x = get_idx(1);
    int y = get_idx(2);
    return { x, y };
}
