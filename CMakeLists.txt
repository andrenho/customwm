#
# Initial setup
#

cmake_minimum_required(VERSION 3.0.0)
project(customwm CXX)
set (CMAKE_CXX_STANDARD 20)

#
# Compiler flags
#

set (CMAKE_CXX_FLAGS_DEBUG "-ggdb -Og \
        -Wall -Wextra -Wnull-dereference -Woverloaded-virtual -fasynchronous-unwind-tables \
  		-D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-protector-strong")
set (CMAKE_CXX_FLAGS_RELEASE "-Ofast")

#
# Dependencies
#

set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/X11/lib/pkgconfig")
find_package(PkgConfig REQUIRED)
pkg_check_modules(X11 xcb xcb-image xcb-errors)

#
# Libraries
#

add_library(engine STATIC
        libengine/engine.hh
        libengine/engine.cc
)

add_library(root-x11 STATIC EXCLUDE_FROM_ALL
        libroot/root.hh
        libroot/root.cc
        libroot/x11/root_x11.hh
        libroot/x11/root_x11.cc
        libroot/x11/server_x11.cc
        libroot/x11/server_x11.hh
)
target_compile_options(root-x11 PUBLIC ${X11_CFLAGS_OTHER})
target_include_directories(root-x11 PUBLIC ${X11_INCLUDE_DIRS})

add_library(root-wayland STATIC EXCLUDE_FROM_ALL
        libroot/root.hh
        libroot/root.cc
        libroot/wayland/rootwayland.hh
        libroot/wayland/rootwayland.cc
)
target_compile_definitions(root-wayland PUBLIC WLR_USE_UNSTABLE)
target_compile_options(root-wayland PUBLIC ${WAYLAND_CFLAGS_OTHER})
target_include_directories(root-wayland PUBLIC ${WAYLAND_INCLUDE_DIRS})

#
# Lua bytecode -> Header
#

set(CUSTOMWM_LUA_BYTECODE ${PROJECT_SOURCE_DIR}/customwm/customwm)
add_custom_command(
        OUTPUT ${CUSTOMWM_LUA_BYTECODE}.hh
        COMMAND luac -o ${CUSTOMWM_LUA_BYTECODE}.luac ${CUSTOMWM_LUA_BYTECODE}.lua
        COMMAND xxd -n customwm -i ${CUSTOMWM_LUA_BYTECODE}.luac > ${CUSTOMWM_LUA_BYTECODE}.hh
        COMMAND rm ${CUSTOMWM_LUA_BYTECODE}.luac
        DEPENDS ${CUSTOMWM_LUA_BYTECODE}.lua
)

#
# Executables
#

set(SRC_CUSTOM_WM
        customwm/main.cc
        customwm/options.hh
        customwm/options.cc
        customwm/windowmanager.hh
        customwm/windowmanager.cc
)

add_executable(customwm-x11 EXCLUDE_FROM_ALL ${SRC_CUSTOM_WM})
target_link_directories(customwm-x11 PUBLIC ${X11_LIBRARY_DIRS})
target_link_libraries(customwm-x11 engine root-x11 ${X11_LIBRARIES})
target_link_libraries(customwm-x11 theme root-x11 ${X11_LIBRARIES})
target_compile_definitions(customwm-x11 PUBLIC BACKEND_X11)

add_executable(customwm-wayland EXCLUDE_FROM_ALL ${SRC_CUSTOM_WM})
target_link_libraries(customwm-wayland theme root-wayland)
add_executable(customwm-wayland EXCLUDE_FROM_ALL ${SRC_CUSTOM_WM})
target_link_directories(customwm-wayland PUBLIC ${WAYLAND_LIBRARY_DIRS})
target_link_libraries(customwm-wayland theme root-wayland ${WAYLAND_LIBRARIES})
target_compile_definitions(customwm-wayland PUBLIC BACKEND_WAYLAND)
