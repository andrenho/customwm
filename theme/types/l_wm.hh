#ifndef L_WM_HH_
#define L_WM_HH_

#include <string>
#include "l_window.hh"

struct L_WM {
    virtual ~L_WM() = default;
    [[nodiscard]] virtual std::string interface_name() const = 0;

    virtual void move_with_window(L_Window* window, bool move) = 0;

    static constexpr const char* mt_identifier = "WM";
};

void l_wm_create_metadata(struct lua_State* L);

#endif //L_WM_HH_
