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
    void load_from_file(std::string const& file);

    bool theme_file_modified() const;

private:
    lua_State *L;
    std::string loaded_file_;
    time_t      last_modification_ = 0;

    void merge_theme();
};

#endif //THEME_HH_
