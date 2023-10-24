#
# configuration
#

CPPFLAGS = -Wall -Wextra -I. -I contrib/luaw/luaw
CXXFLAGS = -std=c++20

ifdef DEBUG
	CPPFLAGS += -O0 -Og -ggdb
else
	CPPFLAGS += -Ofast
	LUAHZ_FLAGS = -s
endif

LDFLAGS = `pkg-config --libs zlib`

LUAW_PATH=contrib/luaw

#
# targets
#

all: customwm-x11

customwm/main-x11.o: customwm/main.cc customwm/customwm.embed
	$(CXX) -c -o $@ $< ${CXXFLAGS} ${CPPFLAGS} -DGRAPHICS=X11

customwm-x11: customwm/main-x11.o customwm/options.o libluaw-jit.a libtheme.a libgraphics-x11.a
	$(CXX) -o $@ $^ ${LDFLAGS}

#
# lua bytecode compilation
#

%.embed: %.lua luazh-jit
	./luazh-jit $(basename $(notdir $<))_lua ${LUASZ_FLAGS} $< > $@

#
# libtheme
#

theme/theme.o: theme/themehelper.embed libluaw-jit.a

libtheme.a: theme/theme.o
	ar -rc $@ $^

#
# libgraphics-x11
#

libgraphics-x11.a: CPPFLAGS += `pkg-config --cflags xcb xcb xcb-image xcb-errors`
libgraphics-x11.a: graphics/graphics.o graphics/x11/graphicsx11.o graphics/x11/wmx11.o
	ar -rc $@ $^

#
# external static libraries / programs
#

luazh-jit:
	$(MAKE) -C ${LUAW_PATH} $@
	cp ${LUAW_PATH}/$@ $@

libluaw-jit.a:
	$(MAKE) -C ${LUAW_PATH} $@ DEBUG=${DEBUG}
	cp ${LUAW_PATH}/$@ $@

#
# tests
#

test-theme: theme/test/test.o libtheme.a libluaw-jit.a
	$(CXX) -o $@ $^ ${LDFLAGS}

check: test-theme
	./test-theme

#
# clean targets
#

clean:
	find . -name '*.embed' -delete
	rm -f *.a theme/*.o customwm/*.o graphics/*.o graphics/x11/*.o graphics/wayland/*.o luazh-jit customwm-x11 customwm-wayland

distclean: clean
	$(MAKE) -C ${LUAW_PATH} clean
