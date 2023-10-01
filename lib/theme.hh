#ifndef THEME_HH_
#define THEME_HH_

#include <functional>
#include <optional>
#include <memory>
#include <string>
#include <variant>
#include "types.hh"

extern "C" {
#include <lua.h>
}

class Theme {
public:
    Theme();

    void load(std::string const& theme_name);

    Padding window_border_width() const { return read_padding("window.border_width"); }

private:
    std::unique_ptr<lua_State, std::function<void(lua_State *)>> L_ptr;
    lua_State *L;

    void load_theme_file(std::string const& filename);

    Padding read_padding(std::string const& prop_name) const;

    bool push_property(std::string const& prop_name) const;

    static std::optional<std::string> find_file(std::string const& theme_file) ;

    static constexpr const char* theme_paths[] = {
            "../themes",
            "./themes",
    };

    void empty_stack() const;
};

#endif //THEME_HH_
