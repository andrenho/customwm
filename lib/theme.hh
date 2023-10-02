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
    void reload_if_modified();

    Padding           read_padding(std::string const& prop_name, Window const& w, std::optional<Padding> default_value={}) const;
    WindowStartingPos read_starting_pos(std::string const& prop_name, Window const& w) const;

private:
    std::unique_ptr<lua_State, std::function<void(lua_State *)>> L_ptr;
    lua_State   *L;
    std::string theme_filename_;
    time_t      loaded_theme_last_modified_;

    void   load_theme_file(std::string const& filename);
    time_t theme_file_last_modified();
    static std::optional<std::string> find_file(std::string const& theme_file);

    void empty_stack() const;
    bool push_property(std::string const& prop_name) const;
    void push_window(const Window &window) const;
    Point to_point(int n) const;

    bool call_lua_window_function(const Window &window) const;

    static constexpr const char* theme_paths[] = {
            "../themes",
            "./themes",
    };
};

#endif //THEME_HH_
