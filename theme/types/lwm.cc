#include "lwm.hh"

#include "luaw.hh"
#include "common/logger.hh"

#define THIS (luaw_to<LWindowManager*>(L, 1))

const std::unordered_map<std::string, GrabType> LWindowManager::grab_types = {
        { "move",         GrabType::Move },
        { "top-left",     GrabType::TopLeft },
        { "top",          GrabType::Top },
        { "top-right",    GrabType::TopRight },
        { "left",         GrabType::Left },
        { "right",        GrabType::Right },
        { "bottom-left",  GrabType::BottomLeft },
        { "bottom",       GrabType::Bottom },
        { "bottom-right", GrabType::BottomRight },
};

void l_wm_create_metadata(lua_State* L)
{
    std::string mt = luaw_set_metatable<LWindowManager>(L, {
        { "interface_name", [](lua_State* L) {
            return luaw_push(L, THIS->interface_name());
        }},
        { "move_window_with_mouse", [](lua_State* L) {
            std::optional<LWindow *> window {};
            if (lua_gettop(L) >= 3)
                window = luaw_to<std::optional<LWindow*>>(L, 3);
            THIS->move_window_with_mouse(luaw_to<bool>(L, 2), window);
            return 0;
        }},
        { "set_focus", [](lua_State* L) {
            std::optional<LWindow *> window {};
            if (lua_gettop(L) >= 2)
                window = luaw_to<std::optional<LWindow*>>(L, 2);
            THIS->set_focus(window);
            return 0;
        }},
        { "grab", [](lua_State* L) {
            auto window = luaw_to<LWindow*>(L, 2);
            if (lua_isnil(L, 3)) {
                THIS->grab(window, GrabType::None);
            } else {
                try {
                    THIS->grab(window, LWindowManager::grab_types.at(luaw_to<std::string>(L, 3)));
                } catch (std::out_of_range&) {
                    luaL_error(L, "Invalid grab type '%s'", luaw_to<const char*>(L, 3));
                }
            }
            return 0;
        }},
        { "close_window", [](lua_State* L) {
            THIS->close_window(luaw_to<LWindow*>(L, 2));
            return 0;
        }},
    });
    LOG.debug("Metatable setup for WM as '%s'", mt.c_str());
}