#include "theme.hh"

#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <sstream>

#include <lauxlib.h>
#include <sys/stat.h>

Theme::Theme()
    : L_ptr(luaL_newstate(), [](lua_State* L) { lua_close(L); }), L(L_ptr.get())
{
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

std::optional<std::string> Theme::find_file(const std::string &theme_file)
{
    struct stat buffer {};
    for (const char* path : theme_paths) {
        std::string full_path = std::string(path) + "/" + path + ".customvm_theme";
        if (stat(full_path.c_str(), &buffer) == 0)
            return full_path;
    }

    return {};
}

void Theme::load_theme_file(const std::string &filename)
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
}

