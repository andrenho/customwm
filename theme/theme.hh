#ifndef THEME_HH_
#define THEME_HH_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "luaw.hh"

class Theme {
public:
    Theme();

    void load_theme(LuaCompressedBytecode lsb[]);
    void load_theme_file(std::string const& filename);
    void load_theme_code(std::string const& code);

    template <typename T> T get_prop(std::string const& prop, auto&&... args) const;
    void call_opt(std::string const& prop, auto&&... args);

    using Fonts = std::map<std::string, std::vector<std::string>>;
    Fonts fonts() const;

    template <typename T> void create_global_object(std::string const& lua_name, T* object);

    void print_effective_theme() const;

private:
    std::unique_ptr<lua_State, std::function<void(lua_State*)>> Lptr;
    lua_State* L;

    void merge_theme();

    static constexpr const char* THEME_GLOBAL = "theme";
};

#include "theme.inl"

#endif //THEME_HH_
