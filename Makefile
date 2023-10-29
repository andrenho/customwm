#
# sources
#

OBJ_THEME = theme/logger.o theme/theme.o theme/types/types.o theme/types/l_wm.o \
            theme/types/l_window.o

OBJ_GRAPHICS = graphics/graphics.o

OBJ_GRAPHICS_X11 = ${OBJ_GRAPHICS} graphics/x11/graphicsx11.o graphics/x11/resourcesx11.o \
                   graphics/x11/windowx11.o graphics/x11/wmx11.o

OBJ_CUSTOMWM = customwm/main.o customwm/options.o

#
# config
#

CPPFLAGS = -Wall -Wextra -I. -I./contrib/luaw/luaw -MD -MP $(shell pkg-config --cflags zlib)
CPPFLAGS_X11 = $(shell pkg-config --cflags x11 xft) -DBACKEND=X11
CXXFLAGS = -std=c++20

ifdef DEBUG
	CPPFLAGS += -O0 -Og -ggdb
else
	CPPFLAGS += -Ofast
	LUAHZ_FLAGS = -s
endif

LDFLAGS = $(shell pkg-config --libs zlib)
LDFLAGS_X11 = ${LDFLAGS} $(shell pkg-config --libs x11 xft)

LUAW_PATH=contrib/luaw
LUAW_FILE=${LUAW_PATH}/README.md
LUAJIT_HEADER=${LUAW_PATH}/luajit/src/luajit.h

#
# main dependencies
#

all: customwm-x11

customwm-x11: CPPFLAGS += ${CPPFLAGS_X11}
customwm-x11: ${OBJ_THEME} ${OBJ_GRAPHICS_X11} ${OBJ_CUSTOMWM} libluaw-jit.a
	$(CXX) -o $@ $^ ${LDFLAGS_X11}

#
# secondary dependencies
#

customwm/main.o: customwm/customwm.embed

theme/theme.o: theme/themehelper.embed

${OBJ_THEME} ${OBJ_GRAPHICS_X11} ${OBJ_CUSTOMWM}: ${LUAJIT_HEADER}

#
# automatic rules
#

%.embed: %.lua ./luazh-jit
	./luazh-jit $(basename $(notdir $<))_lua ${LUASZ_FLAGS} $< > $@

-include ${OBJ:%.o=%.d}

#
# external dependencies
#

${LUAW_FILE}:  # download luaw git submodule, if not there
	git submodule update --recursive --remote

luazh-jit: ${LUAW_FILE} libluaw-jit.a
	$(MAKE) -C ${LUAW_PATH} $@
	cp ${LUAW_PATH}/$@ $@

libluaw-jit.a: ${LUAW_FILE}
	$(MAKE) -C ${LUAW_PATH} $@ DEBUG=${DEBUG}
	cp ${LUAW_PATH}/$@ $@

${LUAJIT_HEADER}: libluaw-jit.a

#
# special targets
#

clean:
	rm -f ${OBJ_THEME} ${OBJ_GRAPHICS_X11} ${OBJ_CUSTOMWM} customwm-x11 **/*.embed **/*.d

distclean: clean
	rm -f libluaw-jit.a luazh-jit
	$(MAKE) -C ${LUAW_PATH} clean
