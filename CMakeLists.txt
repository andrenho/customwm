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

pkg_check_modules(LUA REQUIRED lua5.4)
pkg_check_modules(X11 xcb xcb-image xcb-errors)
pkg_check_modules(WAYLAND wayland-server wlroots pixman-1)

include_directories(${LUA_INCLUDE_DIRS})

#
# Lua bytecode -> Header
#

macro(LuaBytecodeHeader base_path struct_name)
    add_custom_command(
            OUTPUT ${base_path}.hh
            COMMAND luac -o ${base_path}.luac ${base_path}.lua
            COMMAND xxd -n ${struct_name} -i ${base_path}.luac > ${base_path}.hh
            COMMAND rm ${base_path}.luac
            DEPENDS ${base_path}.lua
    )
endmacro()

LuaBytecodeHeader(${PROJECT_SOURCE_DIR}/customwm/customwm customwm)
LuaBytecodeHeader(${PROJECT_SOURCE_DIR}/libengine/helper helper)

#
# Libraries
#

add_library(engine STATIC
        libengine/engine.hh
        libengine/engine.cc
        libengine/helper.hh
        libengine/luaw.hh
        libengine/luaw.inl
        libengine/luaw.cc
        libengine/metatables.cc
        libengine/metatables.hh
        libengine/engine.inl
        libengine/exceptions.hh
        libengine/types/types.hh
        libengine/types/types.cc
)
target_compile_options(engine PUBLIC ${LUA_CFLAGS_OTHER})

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
        libroot/wayland/root_wayland.hh
        libroot/wayland/root_wayland.cc
        libroot/wayland/server_wayland.hh
        libroot/wayland/server_wayland.cc
        libroot/wayland/output.hh
        libroot/wayland/output.cc
)
target_compile_definitions(root-wayland PUBLIC WLR_USE_UNSTABLE)
target_compile_options(root-wayland PUBLIC ${WAYLAND_CFLAGS_OTHER})
target_include_directories(root-wayland PUBLIC ${WAYLAND_INCLUDE_DIRS})

#
# Executables
#

set(SRC_CUSTOM_WM
        customwm/main.cc
        customwm/customwm.hh)

add_executable(customwm-x11 EXCLUDE_FROM_ALL ${SRC_CUSTOM_WM})
target_link_directories(customwm-x11 PUBLIC ${X11_LIBRARY_DIRS} ${LUA_LIBRARY_DIRS})
target_link_libraries(customwm-x11 engine root-x11 ${X11_LIBRARIES} ${LUA_LIBRARIES})
target_compile_definitions(customwm-x11 PUBLIC BACKEND_X11)

add_executable(customwm-wayland EXCLUDE_FROM_ALL ${SRC_CUSTOM_WM})
target_link_directories(customwm-wayland PUBLIC ${WAYLAND_LIBRARY_DIRS} ${LUA_LIBRARY_DIRS})
target_link_libraries(customwm-wayland theme root-wayland ${WAYLAND_LIBRARIES} ${LUA_LIBRARIES})
target_compile_definitions(customwm-wayland PUBLIC BACKEND_WAYLAND)
