#ifndef L_WM_HH_
#define L_WM_HH_

#include <optional>
#include <string>
#include "l_window.hh"

struct L_WM {
    virtual ~L_WM() = default;
    [[nodiscard]] virtual std::string interface_name() const = 0;

    virtual void move_window_with_mouse(bool move, std::optional<L_Window*> window) = 0;
    virtual std::string window_name(L_Window*) const { return "Unnamed Application"; }

    static constexpr const char* mt_identifier = "WM";
};

void l_wm_create_metadata(struct lua_State* L);

#endif //L_WM_HH_
