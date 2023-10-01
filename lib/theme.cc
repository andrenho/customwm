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
    if (r != LUA_OK) {
        fprintf(stderr, "Error loading theme file.\n");
        exit(EXIT_FAILURE);
    }

    if (lua_gettop(L) != 1) {
        fprintf(stderr, "The theme file doesn't return anything. Make sure the syntax is `return { ... }`.\n");
        exit(EXIT_FAILURE);
    }
}

Padding Theme::read_padding(std::string const& prop_name) const
{
    if (!push_property(prop_name))
        throw PropertyNotFoundException(prop_name);

    empty_stack();

    return Padding();
}

bool Theme::push_property(std::string const& prop_name) const
{
    std::istringstream iss(prop_name);
    std::string property;

    while (std::getline(iss, property, '.')) {
        int type = lua_getfield(L, -1, property.c_str());
        if (type == LUA_TNIL || (iss.peek() == EOF /* is last */ && type != LUA_TTABLE)) {
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
