#
# configuration
#

CPPFLAGS = -Wall -Wextra -I luaw/luaw
CXXFLAGS = -std=c++20

ifdef DEBUG
	CPPFLAGS += -O0 -Og -ggdb
else
	CPPFLAGS += -Ofast
endif

#
# main targets
#

TARGETS = customwm-x11
all: ${TARGETS}

customwm-x11: customwm/main.o libtheme.a luaw/libluaw-jit.a
	$(CXX) -o $@ $^ ${LDFLAGS}

#
# dependencies
#

customwm/main.o: theme/theme-helper.embed

theme/theme-helper.embed: theme/theme-helper.lua luaw/luazh-jit
	luaw/luazh-jit theme_helper $< > $@

libtheme.a: theme/theme.o theme/theme-helper.embed
	ar -rc $@ $<

luaw/luazh-jit:
	$(MAKE) -C luaw luazh-jit

luaw/libluaw-jit.a:
	$(MAKE) -C luaw libluaw-jit.a

#
# other targets
#

clean:
	rm -rf *.a *.o **/*.o **/*.embed ${TARGETS}

dist-clean: clean
	$(MAKE) -C luaw clean