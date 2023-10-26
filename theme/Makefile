include ../config.mk

all: libtheme.a

OBJ := theme.o logger.o types/types.o types/l_wm.o types/l_window.o

theme.o: themehelper.embed

libtheme.a: ${OBJ}
	ar -rc $@ $^

clean:
	rm -f ${OBJ} ${OBJ:%.o=%.d} libtheme.a themehelper.embed

-include ${OBJ:%.o=%.d}