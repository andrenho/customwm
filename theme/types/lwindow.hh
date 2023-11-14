#ifndef LWINDOW_HH_
#define LWINDOW_HH_

#include <cstdint>
#include <optional>
#include <string>
#include "types.hh"

using WHandle = uintptr_t;

enum class WindowState { Normal, Minimized, Maximized };

struct LWindow {
    virtual ~LWindow() = default;

    [[nodiscard]] virtual WHandle                id() const = 0;
    [[nodiscard]] virtual std::optional<WHandle> child_id() const = 0;
    [[nodiscard]] virtual Padding                padding() const = 0;
    [[nodiscard]] virtual Rectangle              rect(bool update_cache=false) const = 0;
    [[nodiscard]] virtual std::string            name() const { return "Unnamed Application"; }
    [[nodiscard]] virtual bool                   focused() const = 0;

    virtual void fill(Color const& color, std::optional<Rectangle> rect={}) = 0;
    virtual void text(int x, int y, std::string const& text, TextProperties const& text_properties) = 0;
    virtual void draw(int x, int y, std::string const& slice) = 0;
    virtual void set_cursor(std::string const& key) = 0;
    virtual void move(Point const& new_pos) = 0;
    virtual void resize(Size const& new_size) = 0;
    virtual void maximize_restore() = 0;
    virtual void close() = 0;

    static constexpr const char* mt_identifier = "Window";

protected:
};

void l_window_create_metadata(lua_State* L);

#endif //LWINDOW_HH_
