#ifndef LWINDOW_HH_
#define LWINDOW_HH_

#include <cstdint>
#include <optional>
#include <string>
#include "types.hh"

struct LWindow {
    virtual ~LWindow() = default;

    [[nodiscard]] virtual Rectangle rect() const = 0;
    virtual void        fill(Color const& color) = 0;
    virtual void        text(int x, int y, std::string const& text, TextProperties const& text_properties) = 0;
    virtual void        draw(int x, int y, std::string const& slice) = 0;
    virtual std::string name() const { return "Unnamed Application"; }
    virtual void        set_cursor(std::string const& key) = 0;

    static constexpr const char* mt_identifier = "Window";
};

void l_window_create_metadata(lua_State* L);

#endif //LWINDOW_HH_