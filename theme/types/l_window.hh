#ifndef L_WINDOW_HH_
#define L_WINDOW_HH_

#include <cstdint>
#include <string>
#include "types.hh"

struct L_Window {
    virtual ~L_Window() = default;

    [[nodiscard]] virtual Rectangle rect() const = 0;
    virtual void fill(Color const& color) = 0;

    static constexpr const char* mt_identifier = "Window";
};

void l_window_create_metadata(lua_State* L);

#endif //L_WINDOW_HH_
