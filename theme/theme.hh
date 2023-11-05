#ifndef THEME_HH_
#define THEME_HH_

#include <functional>
#include <optional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <sys/stat.h>

#include "luaw.hh"
#include "theme/types/types.hh"

enum class ErrorAction { LOG, ERROR, THROW };

class Theme {
public:
    Theme();

    void load_theme(LuaCompressedBytecode lsb[]);
    void load_theme_file(std::string const& filename);
    void load_theme_code(std::string const& code);

    void set_error_action(ErrorAction action);

    template <typename T> T get_prop(std::string const& prop, auto&&... args) const;
    void call_opt(std::string const& prop, auto&&... args);

    std::vector<std::string> resource_font(std::string const& key) const;
    std::optional<Slice> resource_slice(std::string const& key) const;
    std::vector<uint8_t> resource_image(std::string const& key) const;

    template <typename T> void create_global_object(std::string const& lua_name, T* object);

    void print_effective_theme() const;
    void check_for_theme_file_reloaded();

private:
    std::unique_ptr<lua_State, std::function<void(lua_State*)>> Lptr;
    lua_State* L;
    std::vector<std::pair<std::string, long>> watched_files_;

    void merge_theme();
    void add_watched_file(std::string const& filename);

    static constexpr const char* THEME_GLOBAL = "theme";
};

extern Theme theme;

#include "theme.inl"

#endif //THEME_HH_
