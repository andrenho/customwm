#ifndef LWM_HH_
#define LWM_HH_

#include <optional>
#include <string>
#include "lwindow.hh"

struct LWindowManager {
    virtual ~LWindowManager() = default;
    [[nodiscard]] virtual std::string interface_name() const = 0;

    virtual void move_window_with_mouse(bool move, std::optional<LWindow*> window) = 0;
    virtual void set_focus(std::optional<LWindow*> window) = 0;
    virtual void close_window(LWindow* window) = 0;

    static constexpr const char* mt_identifier = "WM";
};

void l_wm_create_metadata(struct lua_State* L);

#endif //LWM_HH_
