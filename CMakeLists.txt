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

add_library(theme STATIC
        libtheme/theme.hh
        libtheme/theme.cc
)

add_library(root-x11 STATIC EXCLUDE_FROM_ALL
        libroot/root.hh
        libroot/x11/root_x11.hh
        libroot/x11/root_x11.cc
        libroot/eventlistener.hh
)
target_compile_options(root-x11 PUBLIC ${X11_CFLAGS_OTHER})
target_include_directories(root-x11 PUBLIC ${X11_INCLUDE_DIRS})

add_library(root-wayland STATIC EXCLUDE_FROM_ALL
        libroot/root.hh
        libroot/wayland/rootwayland.hh
        libroot/wayland/rootwayland.cc
        libroot/eventlistener.hh
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
target_link_libraries(customwm-x11 theme root-x11 ${X11_LIBRARIES})

add_executable(customwm-wayland EXCLUDE_FROM_ALL ${SRC_CUSTOM_WM})
target_link_libraries(customwm-wayland theme root-wayland)
