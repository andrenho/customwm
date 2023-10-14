cmake_minimum_required(VERSION 3.26)
project(customwm)

set(CMAKE_CXX_STANDARD 20)

set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/X11/lib/pkgconfig")
find_package(PkgConfig)
find_package(Lua REQUIRED)
pkg_check_modules(XCB REQUIRED xcb xcb-image xcb-errors)

link_directories(${XCB_LIBRARY_DIRS})
link_libraries(${LUA_LIBRARIES} ${XCB_LIBRARIES})
include_directories(${LUA_INCLUDE_DIR} ${XCB_INCLUDE_DIRS})

set(COMPILED_BASE_THEME ${CMAKE_CURRENT_SOURCE_DIR}/libtheme/base/base)
add_custom_command(
        OUTPUT ${COMPILED_BASE_THEME}.hh
        COMMAND luac -o ${COMPILED_BASE_THEME}.luac ${COMPILED_BASE_THEME}.lua
        COMMAND xxd -i ${COMPILED_BASE_THEME}.luac > ${COMPILED_BASE_THEME}.hh
        COMMAND rm ${COMPILED_BASE_THEME}.luac
        DEPENDS ${COMPILED_BASE_THEME}.lua
)

add_library(customwm-libtheme STATIC
        libtheme/theme.hh
        libtheme/theme.cc
        libtheme/exceptions.hh
        libtheme/luaw.hh
        libtheme/luaw.cc
        libtheme/types/types.cc
        libtheme/types/types.hh
        libtheme/theme.inl
        libtheme/types/iwindow.hh
        libtheme/types/iwindow.cc
        libtheme/types/resources.cc
        libtheme/types/resources.hh
        ${COMPILED_BASE_THEME}.hh
)

add_library(customwm-libresource STATIC
        libresource/resourcemanager.cc
        libresource/resourcemanager.hh
        libresource/contrib/stb_image.h
        libresource/colors.cc
        libresource/colors.hh
)

add_executable(customwm
        wm/main.cc
        wm/options.cc
        wm/options.hh
        wm/window.hh
        wm/wm.cc
        wm/wm.hh
        wm/window.cc
)
target_link_libraries(customwm customwm-libtheme customwm-libresource)
