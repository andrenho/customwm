cmake_minimum_required(VERSION 3.26)
project(customwm)

set(CMAKE_CXX_STANDARD 20)

find_package(X11 REQUIRED)
find_package(Lua REQUIRED)

link_libraries(${X11_xcb_LIB} ${LUA_LIBRARIES})
include_directories(${X11_xcb_INCLUDE_PATH} ${LUA_INCLUDE_DIR})

add_library(customwm-libtheme STATIC
        libtheme/theme.hh
        libtheme/theme.cc
)

set(COMPILED_BASE_THEME ${CMAKE_CURRENT_SOURCE_DIR}/wm/base/base)
add_custom_command(
        OUTPUT ${COMPILED_BASE_THEME}.hh
        COMMAND luac -o ${COMPILED_BASE_THEME}.luac ${COMPILED_BASE_THEME}.lua
        COMMAND xxd -i -n base_lua ${COMPILED_BASE_THEME}.luac > ${COMPILED_BASE_THEME}.hh
        COMMAND rm ${COMPILED_BASE_THEME}.luac
        DEPENDS ${COMPILED_BASE_THEME}.lua
)
add_custom_target(base DEPENDS ${COMPILED_BASE_THEME}.hh)

add_executable(customwm
        wm/main.cc
        wm/options.cc
        wm/options.hh
        wm/base/base.hh
)
add_dependencies(customwm base)
target_link_libraries(customwm customwm-libtheme)
