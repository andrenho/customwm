#
# sources
#

OBJ_COMMON = util/logger.o util/init.o util/options.o \
	theme/theme.o theme/types/types.o theme/types/l_wm.o theme/types/l_window.o

OBJ_GRAPHICS_X11 = graphics/x11/x11.o graphics/x11/resources.o graphics/x11/xwindow.o graphics/x11/wm.o

OBJ_CUSTOMWM = customwm/main.o

#
# config
#

CPPFLAGS = -Wall -Wextra -I. -I./contrib/luaw/luaw -MD -MP $(shell pkg-config --cflags zlib)
CPPFLAGS_X11 = $(shell pkg-config --cflags x11 xft) -DBACKEND=X11
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

util/init.o: util/base_theme.embed

theme/theme.o: theme/themehelper.embed

${OBJ_COMMON} ${OBJ_GRAPHICS_X11} ${OBJ_CUSTOMWM}: ${LUAJIT_HEADER}

#
# automatic rules
#

%.embed: %.lua ${LUAZH}
	${LUAZH} $(basename $(notdir $<))_lua ${LUASZ_FLAGS} $< > $@

-include ${OBJ:%.o=%.d}

#
# external dependencies
#

${LUAW_FILE}:  # download luaw git submodule, if not there
	git submodule update --init --recursive

${LUAZH}: ${LUAW_FILE} ${LUAJIT_A}
	$(MAKE) -C ${LUAW_PATH} $@

${LUAJIT_A}: ${LUAW_FILE}
	$(MAKE) -C ${LUAW_PATH} $@ DEBUG=${DEBUG}

${LUAJIT_HEADER}: ${LUAJIT_A}

#
# special targets
#

clean:
	find . -name '*.embed' -delete
	find . -name '*.d' -delete
	rm -f ${OBJ_COMMON} ${OBJ_GRAPHICS_X11} ${OBJ_CUSTOMWM} customwm-x11

distclean: clean
	$(MAKE) -C ${LUAW_PATH} clean
