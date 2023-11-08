#include "theme.hh"

#include <cstdlib>
#include <sys/stat.h>
#include <ios>
#include <fstream>

#include "themeexception.hh"
#include "themehelper.embed"
#include "types/exceptions.hh"
#include "types/l_wm.hh"
#include "types/l_window.hh"
#include "../util/logger.hh"

Theme THEME;

Theme::Theme()
    : Lptr(luaw_newstate(), [](lua_State* LL) { lua_close(LL); }), L(Lptr.get())
{
    set_error_action(ErrorAction::THROW);

    luaw_do_z(L, themehelper_lua);

    l_wm_create_metadata(L);
    l_window_create_metadata(L);
}

void Theme::set_error_action(ErrorAction action)
{
    switch (action) {
        case ErrorAction::LOG:
            lua_atpanic(L, [](lua_State* LL) -> int {
                LOG.error("lua error: %s", lua_tostring(LL, -1));
                return 0;
            });
            break;
        case ErrorAction::ERROR:
            lua_atpanic(L, [](lua_State* LL) -> int {
                LOG.error("lua error: %s", lua_tostring(LL, -1));
                exit(EXIT_FAILURE);
            });
            break;
        case ErrorAction::THROW:
            lua_atpanic(L, [](lua_State* LL) -> int {   // makes debugging easier
                throw std::runtime_error(std::string("lua error: ") + lua_tostring(LL, -1));
            });
            break;
    }
}


void Theme::load_theme([[maybe_unused]] LuaCompressedBytecode lsb[])
{
    int top = lua_gettop(L);

#ifdef DEBUG
    luaw_dofile(L, "./theme/themehelper.lua", 0, "theme");
    add_watched_file("./theme/themehelper.lua");
#else
    luaw_do_z(L, lsb, true);
#endif

    if (lua_gettop(L) != top + 1)
        throw ThemeException("Theme file does not return a table.");

    merge_theme();
}

void Theme::load_theme_file(std::string const &filename)
{
    luaw_dofile(L, filename, 1);
    merge_theme();
    add_watched_file(filename);
}

void Theme::load_theme_code(std::string const &code)
{
    luaw_do(L, code, 1);
    merge_theme();
}

void Theme::print_effective_theme() const
{
    lua_getglobal(L, THEME_GLOBAL);
    printf("%s\n", luaw_dump(L, -1, true, 10).c_str());
    lua_pop(L, 1);
}

void Theme::merge_theme()
{
    lua_getglobal(L, "merge_theme");
    lua_pushvalue(L, -2);
    lua_call(L, 1, 0);
    lua_pop(L, 1);
}

std::vector<std::string> Theme::resource_font(std::string const& key) const
{
    lua_getglobal(L, THEME_GLOBAL);
    if (!luaw_hasfield(L, -1, "resources.fonts." + key)) {
        lua_pop(L, 1);
        return {};
    }

    auto r = luaw_getfield<std::vector<std::string>>(L, -1, "resources.fonts." + key);
    lua_pop(L, 1);
    return r;
}

std::optional<Slice> Theme::resource_slice(std::string const &key) const
{
    lua_getglobal(L, THEME_GLOBAL);
    if (!luaw_hasfield(L, -1, "resources.slices." + key)) {
        lua_pop(L, 1);
        return {};
    }

    auto r = luaw_getfield<Slice>(L, -1, "resources.slices." + key);
    lua_pop(L, 1);
    return r;
}

void Theme::add_watched_file(std::string const& filename)
{
    struct stat result {};
    if (stat(filename.c_str(), &result) == 0)
        watched_files_.emplace_back(filename, result.st_mtime);
}

void Theme::check_for_theme_file_reloaded()
{
    for (auto const& [filename, last_modif] : watched_files_) {
        struct stat result {};
        if (stat(filename.c_str(), &result) == 0 && result.st_mtime > last_modif)
            goto restart;
    }
    return;

restart:
    lua_pushnil(L);
    lua_setglobal(L, THEME_GLOBAL);

    auto files = watched_files_;
    watched_files_.clear();

    for (auto& [filename, last_modif] : files) {
        struct stat result {};
        if (stat(filename.c_str(), &result) == 0) {
            last_modif = result.st_mtime;
            load_theme_file(filename);
        }
    }

    throw RestartException();
}

std::vector<uint8_t> Theme::resource_image(std::string const &key) const
{
    lua_getglobal(L, THEME_GLOBAL);
    if (!luaw_hasfield(L, -1, "resources.images." + key))
        throw std::runtime_error("Could not find resource image '" + key + "'");
    luaw_getfield(L, -1, "resources.images." + key);

    if (luaw_hasfield(L, -1, "filename")) {
        std::string filename = luaw_getfield<std::string>(L, -1, "filename");
        std::ifstream stream(filename, std::ios::in | std::ios::binary);
        if (!stream.good())
            throw std::runtime_error("Could not open file '" + filename + "'");
        std::vector<uint8_t> contents { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
        return contents;
    } else {
        throw std::runtime_error("Malformed resource image");
    }
}