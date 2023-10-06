#ifndef THEME_HH_
#define THEME_HH_

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

class Theme {
public:
    Theme();
    ~Theme();

    Theme(Theme const&) = delete;
    Theme& operator=(Theme const&) = delete;
    Theme(Theme&&) = default;
    Theme& operator=(Theme&&) = default;

    void load_from_ram(const unsigned char *data, unsigned int len);

private:
    lua_State *L;
};

#endif //THEME_HH_
