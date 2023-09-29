cmake_minimum_required(VERSION 3.26)
project(customwm)

set(CMAKE_CXX_STANDARD 20)

find_package(X11 REQUIRED)
link_libraries(${X11_xcb_LIB})
include_directories(${X11_xcb_INCLUDE_PATH})

add_executable(customwm wm/main.cc
        wm/config.hh
        wm/config.cc
        wm/wm.hh
        wm/wm.cc)
