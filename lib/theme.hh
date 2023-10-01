#ifndef THEME_HH_
#define THEME_HH_

#include <functional>
#include <optional>
#include <memory>
#include <string>

#include <lua.h>

class Theme {
public:
    Theme();

    void load(std::string const& theme_name);

private:
    std::unique_ptr<lua_State, std::function<void(lua_State *)>> L_ptr;
    lua_State *L;

    void load_theme_file(std::string const& filename);

    static std::optional<std::string> find_file(std::string const& theme_file) ;

    static constexpr const char* theme_paths[] = {
            "../themes",
            "./themes",
    };
};

#endif //THEME_HH_
