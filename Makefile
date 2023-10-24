#
# configuration
#

CPPFLAGS = -Wall -Wextra -I luaw/luaw -I. `pkg-config --cflags zlib`
CXXFLAGS = -std=c++20

ifdef DEBUG
	CPPFLAGS += -O0 -Og -ggdb
else
	CPPFLAGS += -Ofast
endif

LDFLAGS = `pkg-config --libs zlib`

#
# main targets
#

TARGETS = customwm-x11
all: ${TARGETS} test-theme

customwm-x11: customwm/main.o libtheme.a luaw/libluaw-jit.a
	$(CXX) -o $@ $^ ${LDFLAGS}

#
# dependencies
#

theme/theme.o: theme/theme-helper.embed

theme/theme-helper.embed: theme/theme-helper.lua luaw/luazh-jit
	luaw/luazh-jit theme_helper $< > $@

libtheme.a: theme/theme.o
	ar -rc $@ $<

luaw/luazh-jit:
	$(MAKE) -C luaw luazh-jit

luaw/libluaw-jit.a:
	$(MAKE) -C luaw libluaw-jit.a

#
# tests
#
test-theme: theme/test/test.o libtheme.a luaw/libluaw-jit.a
	$(CXX) -o $@ $^ ${LDFLAGS}

check: test-theme
	./test-theme

#
# other targets
#

clean:
	rm -rf *.a *.o **/*.o **/*.embed ${TARGETS}

dist-clean: clean
	$(MAKE) -C luaw clean