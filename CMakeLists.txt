cmake_minimum_required(VERSION 3.12)

project(customwm)

#
# Sources
#

set(SCRIPTS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/theme/scripts)
set(LUA_SCRIPTS
        ${SCRIPTS_PATH}/deepcopy.lua
        ${SCRIPTS_PATH}/inspect.lua
        ${SCRIPTS_PATH}/theme-utils.lua
        ${SCRIPTS_PATH}/base-theme.lua
)

set(COMMON_SRC
        options/options.hh options/options.cc
        theme/theme.hh theme/theme.inl theme/theme.cc theme/themeexception.hh
        theme/types/types.hh theme/types/types.cc
        graphics/windowhandle.hh graphics/event.hh graphics/graphics.hh
        util/log.hh util/log.cc
)

set(X11_SRC
        graphics/x11/xgraphics.hh graphics/x11/xgraphics.cc
        graphics/x11/xresources.hh graphics/x11/xresources.cc
)

set(CUSTOMWM_SRC
        ${COMMON_SRC}
        customwm/main.cc
        windowmanager/windowmanager.hh windowmanager/windowmanager.cc
        windowmanager/wmevents.hh windowmanager/wmevents.cc
        window/window.hh window/window.cc
        window/parentwindow.hh window/parentwindow.cc
)

set(CUSTOMWM_X11_SRC
        ${X11_SRC}
        ${CUSTOMWM_SRC}
        graphics/cursors.hh
)

#
# External libraries
#

find_package(ZLIB REQUIRED)
find_package(Freetype REQUIRED)
find_package(X11 REQUIRED)
# TODO - check for X11/wayland depending on the target

#
# Options
#

set(GENPATH ${CMAKE_CURRENT_BINARY_DIR}/include)
set(LUAW_PATH ${PROJECT_SOURCE_DIR}/contrib/luaw)
set(LUAJIT_H ${LUAW_PATH}/luajit/src/luajit.h)
set(LUAZH_JIT ${LUAW_PATH}/luazh-jit)
set(LUAJIT_A ${LUAW_PATH}/libluaw-jit.a)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)
include_directories(${PROJECT_SOURCE_DIR} ${GENPATH} contrib/luaw/luaw contrib/luaw/luajit/src
        ${X11_INCLUDE_DIR} ${X11_Xft_INCLUDE_PATH} ${FREETYPE_INCLUDE_DIRS})

#
# Clone git submodules
#

if (NOT EXISTS ${LUAW_PATH}/Makefile)
    execute_process(COMMAND git submodule update --init --remote --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY)
endif()

#
# luaw and luajit compilation
#

add_custom_command(
        OUTPUT ${LUAZH_JIT} ${LUAJIT_A}
        COMMAND make -j 10 -C ${PROJECT_SOURCE_DIR}/contrib/luaw luazh-jit libluaw-jit.a
)

add_custom_target(distclean
        COMMAND make clean
        COMMAND make -C ${PROJECT_SOURCE_DIR}/contrib/luaw clean)

#
# lua scripts header generation
#

add_custom_command(
        OUTPUT ${GENPATH}/luascripts.inc
        DEPENDS ${LUAZH_JIT}
        COMMAND mkdir -p ${GENPATH}
        COMMAND ${LUAZH_JIT} luascripts ${LUASZ_FLAGS} ${LUA_SCRIPTS} > ${GENPATH}/luascripts.inc)
add_custom_target(luascripts DEPENDS ${GENPATH}/luascripts.inc)

#
# main executables
#

add_library(luajit_a SHARED IMPORTED)
set_target_properties(luajit_a PROPERTIES IMPORTED_LOCATION ${LUAJIT_A})

add_executable(customwm-x11 ${CUSTOMWM_X11_SRC})
add_dependencies(customwm-x11 luascripts)
target_link_libraries(customwm-x11 PRIVATE luajit_a ${ZLIB_LIBRARIES} ${X11_LIBRARIES} ${X11_Xft_LIB} ${FREETYPE_LIBRARIES})
