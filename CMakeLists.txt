cmake_minimum_required(VERSION 3.26)
project(customwm)

set(CMAKE_CXX_STANDARD 20)

find_package(X11 REQUIRED)
find_package(Lua REQUIRED)

link_libraries(${X11_xcb_LIB} ${LUA_LIBRARIES})
include_directories(${X11_xcb_INCLUDE_PATH} ${LUA_INCLUDE_DIR})

#add_library(customwm-lib STATIC
#        _old/lib/types/ibrush.cc)

add_executable(customwm
        wm/main.cc
        wm/options.cc
        wm/options.hh
)
#target_link_libraries(customwm customwm-lib)
