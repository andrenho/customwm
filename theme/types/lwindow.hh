#ifndef LWINDOW_HH_
#define LWINDOW_HH_

#include <cstdint>
#include <optional>
#include <string>
#include "types.hh"

using WHandle = uintptr_t;

struct LWindow {
    virtual ~LWindow() = default;

    // LUA methods
    virtual WHandle                   id() const = 0;
    std::optional<WHandle>            child_id() const { return child_id_; }
    Padding                           padding() const { return child_padding_; }
    [[nodiscard]] virtual Rectangle   rect(bool update_cache=false) const = 0;
    virtual void                      fill(Color const& color, std::optional<Rectangle> rect={}) = 0;
    virtual void                      text(int x, int y, std::string const& text, TextProperties const& text_properties) = 0;
    virtual void                      draw(int x, int y, std::string const& slice) = 0;
    [[nodiscard]] virtual std::string name() const { return "Unnamed Application"; }
    virtual void                      set_cursor(std::string const& key) = 0;
    virtual bool                      focused() const = 0;
    virtual void                      move(Point const& new_pos) = 0;
    virtual void                      resize(Size const& new_size) = 0;

    // C++ exclusive methods
    void                              set_child(WHandle child_id, Padding const& padding);
    virtual void                      update_rectangle(Rectangle const& r) { rectangle_ = r; }

    static constexpr const char* mt_identifier = "Window";

protected:
    mutable Rectangle      rectangle_ { 0, 0, 0, 0 };
    std::optional<WHandle> child_id_;
    Padding                child_padding_;
};

void l_window_create_metadata(lua_State* L);

#endif //LWINDOW_HH_
