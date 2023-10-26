CPPFLAGS = -Wall -Wextra -I. -MD -MP
CXXFLAGS = -std=c++20

ifdef DEBUG
	CPPFLAGS += -O0 -Og -ggdb
else
	CPPFLAGS += -Ofast
	LUAHZ_FLAGS = -s
endif

%.embed: %.lua ../luazh-jit
	../luazh-jit $(basename $(notdir $<))_lua ${LUASZ_FLAGS} $< > $@
