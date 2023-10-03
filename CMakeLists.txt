cmake_minimum_required(VERSION 3.26)
project(customwm)

set(CMAKE_CXX_STANDARD 20)

find_package(X11 REQUIRED)
find_package(Lua REQUIRED)

link_libraries(${X11_xcb_LIB} ${LUA_LIBRARIES})
include_directories(${X11_xcb_INCLUDE_PATH} ${LUA_INCLUDE_DIR})

add_library(customwm-lib STATIC
        lib/theme.hh
        lib/theme.cc
        lib/types/types.hh
        lib/exceptions.hh
        lib/types/window.hh
        lib/luaw.cc
        lib/luaw.hh
        lib/themefile.cc
        lib/themefile.hh
        lib/types/types.cc
        lib/theme.inl
        lib/types/window.cc
        lib/types/ibrush.hh
        lib/types/ibrush.cc)

add_executable(customwm wm/main.cc
        wm/wm/wm.cc
        wm/wm/wm.hh
        wm/wm/ievents.hh
        wm/x11/x11.cc
        wm/x11/x11.hh
        wm/x11/brush.cc
        wm/x11/brush.hh
)
target_link_libraries(customwm customwm-lib)
