#ifndef THEME_HH_
#define THEME_HH_

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <string>
#include <cstdint>

class Theme {
public:
    Theme();
    ~Theme();

    Theme(Theme const&) = delete;
    Theme& operator=(Theme const&) = delete;
    Theme(Theme&&) = default;
    Theme& operator=(Theme&&) = default;

    void load_from_ram(std::string const& name, uint8_t const* data, unsigned int len);

private:
    lua_State *L;

    void merge_theme();
};

#endif //THEME_HH_
