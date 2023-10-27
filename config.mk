CPPFLAGS = -Wall -Wextra -I. -I${ROOT} -I${ROOT}/contrib/luaw/luaw -MD -MP
CXXFLAGS = -std=c++20

ifdef DEBUG
	CPPFLAGS += -O0 -Og -ggdb
else
	CPPFLAGS += -Ofast
	LUAHZ_FLAGS = -s
endif

LDFLAGS = `pkg-config --libs zlib`
LDFLAGS_X11 = `pkg-config --libs x11 xft`

%.embed: %.lua ../luazh-jit
	../luazh-jit $(basename $(notdir $<))_lua ${LUASZ_FLAGS} $< > $@
