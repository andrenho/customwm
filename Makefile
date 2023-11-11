#
# sources
#

OBJ_COMMON = common/logger.o common/init.o common/options.o \
	theme/theme.o theme/types/types.o theme/types/lwm.o theme/types/lwindow.o \
	graphics/graphics.o graphics/wm.o graphics/focusmanager.o

OBJ_GRAPHICS_X11 = graphics/x11/x.o graphics/x11/xwm.o graphics/x11/xresources.o graphics/x11/xwindow.o

OBJ_CUSTOMWM = customwm/main.o

LUA_SCRIPTS = scripts/deepcopy.lua scripts/theme-utils.lua scripts/inspect.lua scripts/base-theme.lua

#
# config
#

CPPFLAGS = -Wall -Wextra -I. -isystem ./contrib -isystem ./contrib/luaw/luaw -MD -MP $(shell pkg-config --cflags zlib)
CPPFLAGS_X11 = $(shell pkg-config --cflags x11 xft) -DX11
CXXFLAGS = -std=c++20

ifdef DEBUG
	CPPFLAGS += -Og -g3 -DDEBUG=1
else
	CPPFLAGS += -Ofast
	LUAHZ_FLAGS = -s
endif

LDFLAGS = $(shell pkg-config --libs zlib)
LDFLAGS_X11 = ${LDFLAGS} $(shell pkg-config --libs x11 xft)

LUAW_PATH=contrib/luaw
LUAW_FILE=${LUAW_PATH}/README.md

LUAJIT_HEADER=${LUAW_PATH}/luajit/src/luajit.h
LUAJIT_A=contrib/luaw/libluaw-jit.a
LUAZH=contrib/luaw/luazh-jit

#
# main dependencies
#

all: customwm-x11

customwm-x11: CPPFLAGS += ${CPPFLAGS_X11}
customwm-x11: ${OBJ_COMMON} ${OBJ_GRAPHICS_X11} ${OBJ_CUSTOMWM} ${LUAJIT_A}
	$(CXX) -o $@ $^ ${LDFLAGS_X11}

#
# secondary dependencies
#

${OBJ_COMMON} ${OBJ_GRAPHICS_X11} ${OBJ_CUSTOMWM}: ${LUAJIT_HEADER}

-include ${OBJ:%.o=%.d}

#
# scripts
#

theme/theme.o: scripts/luascripts.inc

scripts/luascripts.inc: ${LUA_SCRIPTS} ${LUAZH}
	${LUAZH} luascripts ${LUASZ_FLAGS} ${LUA_SCRIPTS} > $@

#
# external dependencies
#

${LUAW_FILE}:  # download luaw git submodule, if not there
	git submodule update --init --recursive

${LUAZH}: ${LUAW_FILE} ${LUAJIT_A}
	$(MAKE) -C ${LUAW_PATH} luazh-jit

${LUAJIT_A}: ${LUAW_FILE}
	$(MAKE) -C ${LUAW_PATH} libluaw-jit.a DEBUG=${DEBUG}

${LUAJIT_HEADER}: ${LUAJIT_A}

#
# special targets
#

clean:
	find . -name '*.inc' -delete
	find . -name '*.d' -delete
	rm -f ${OBJ_COMMON} ${OBJ_GRAPHICS_X11} ${OBJ_CUSTOMWM} customwm-x11

distclean: clean
	$(MAKE) -C ${LUAW_PATH} clean
