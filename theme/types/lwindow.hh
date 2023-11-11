#ifndef LWINDOW_HH_
#define LWINDOW_HH_

#include <cstdint>
#include <optional>
#include <string>
#include "types.hh"

using WHandle = uintptr_t;

struct LWindow {

    virtual ~LWindow() = default;

    virtual WHandle                   id() const = 0;
    virtual std::optional<WHandle>    child_id() const = 0;
    [[nodiscard]] virtual Rectangle   rect() const = 0;
    virtual void                      fill(Color const& color, std::optional<Rectangle> rect={}) = 0;
    virtual void                      text(int x, int y, std::string const& text, TextProperties const& text_properties) = 0;
    virtual void                      draw(int x, int y, std::string const& slice) = 0;
    [[nodiscard]] virtual std::string name() const { return "Unnamed Application"; }
    virtual void                      set_cursor(std::string const& key) = 0;
    virtual bool                      focused() const = 0;

    static constexpr const char* mt_identifier = "Window";
};

void l_window_create_metadata(lua_State* L);

#endif //LWINDOW_HH_
