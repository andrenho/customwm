#ifndef L_WINDOW_HH_
#define L_WINDOW_HH_

#include <cstdint>
#include <optional>
#include <string>
#include "types.hh"

struct L_Window {
    virtual ~L_Window() = default;

    [[nodiscard]] virtual Rectangle rect() const = 0;
    virtual void        fill(Color const& color) = 0;
    virtual void        text(int x, int y, std::string const& text, TextProperties const& text_properties) = 0;
    virtual void        draw(int x, int y, std::string const& slice) = 0;
    virtual std::optional<std::string> name() const = 0;

    static constexpr const char* mt_identifier = "Window";
};

void l_window_create_metadata(lua_State* L);

#endif //L_WINDOW_HH_
