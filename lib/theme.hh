#ifndef THEME_HH_
#define THEME_HH_

#include <functional>
#include <optional>
#include <memory>
#include <string>
#include <variant>
#include "types/window.hh"
#include "themefile.hh"

extern "C" {
#include <lua.h>
}

class Theme {
public:
    explicit Theme(std::string const& theme_name);

    void reload_if_modified();

    template<typename T>
    T read(std::string const &prop_name, std::optional<Window const*> w={}, std::optional<T> default_value = {}) const;

    template<typename... Types>
    void call(std::string const& prop_name, Types... args);

private:
    std::unique_ptr<lua_State, std::function<void(lua_State *)>> L_ptr;
    lua_State   *L;
    ThemeFile   theme_file_;

    void   load_theme_file();
};

#include "theme.inl"

#endif //THEME_HH_
