cmake_minimum_required(VERSION 3.26)
project(customwm)

set(CMAKE_CXX_STANDARD 20)

find_package(X11 REQUIRED)
find_package(Lua REQUIRED)

link_libraries(${X11_xcb_LIB} ${LUA_LIBRARIES})
include_directories(${X11_xcb_INCLUDE_PATH} ${LUA_INCLUDE_DIR})

add_library(customwm-lib STATIC
        lib/customwmlib.hh
        lib/customwmlib.cc
        lib/theme.hh
        lib/theme.cc
        lib/types.hh
        lib/exceptions.hh)

add_executable(customwm wm/main.cc
        wm/wm.cc
        wm/wm.hh
        wm/windows.cc
        wm/windows.hh
        wm/wm_x11.cc
        wm/wm_x11.hh
)
target_link_libraries(customwm customwm-lib)
