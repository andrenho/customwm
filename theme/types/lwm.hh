#ifndef LWM_HH_
#define LWM_HH_

#include <unordered_map>
#include <optional>
#include <string>
#include "lwindow.hh"

enum class GrabType {
    NoGrab, Move, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight,
};

struct LWindowManager {
    virtual ~LWindowManager() = default;
    [[nodiscard]] virtual std::string interface_name() const = 0;

    virtual void set_focus(std::optional<LWindow*> window) = 0;
    virtual void close_window(LWindow* window) = 0;
    virtual void grab(LWindow* window, GrabType grab_type, Point const& initial_pos) = 0;

    virtual Size screen_size() const = 0;

    static constexpr const char* mt_identifier = "WM";
    static const std::unordered_map<std::string, GrabType> grab_types;
};

void l_wm_create_metadata(struct lua_State* L);

#endif //LWM_HH_
