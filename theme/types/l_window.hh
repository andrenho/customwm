#ifndef L_WINDOW_HH_
#define L_WINDOW_HH_

#include <cstdint>
#include <string>

struct L_Window {
    uintptr_t parent_id;
    uintptr_t child_id;
    int32_t  x, y;
    uint32_t w, h;

    void to_lua(struct lua_State* L) const;

    [[nodiscard]] std::string to_string() const;
};

void l_window_create_metadata(lua_State* L);

#endif //L_WINDOW_HH_
