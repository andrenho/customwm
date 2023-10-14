TARGETS :=
ifeq ($(BACKEND),x11)
	TARGETS += customwm-x11
endif
ifeq ($(BACKEND),wayland)
	TARGETS += customwm-wayland
endif

ifeq (${TARGETS},)
  $(warning Please choose a backend (x11 or wayland). Example: make BACKEND=x11)
  TARGETS = customwm-dummy
endif

CXXFLAGS := -Wall -Wextra
ifdef RELEASE
	CXXFLAGS += -O3
else
	CXXFLAGS += -ggdb -O0
endif

OBJ_CUSTOMWM := customwm/main.o

all: ${TARGETS}

customwm-x11: ${OBJ_CUSTOMWM}
	g++ -o $@ $^ ${LDFLAGS}

customwm-wayland: ${OBJ_CUSTOMWM}
	g++ -o $@ $^ ${LDFLAGS}

clean:
	rm -f ${TARGETS} ${OBJ_CUSTOMWM}