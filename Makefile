all: customwm-x11

FORCE: ;

export ROOT=$(shell pwd)

include config.mk

#
# internal libraries dependencies
#

theme/libtheme.a: libluaw-jit.a luazh-jit FORCE
	${MAKE} -C theme all DEBUG=${DEBUG}

graphics/libgraphics-x11.a: FORCE
	${MAKE} -C graphics -f Makefile.x11 all DEBUG=${DEBUG}

customwm-x11: theme/libtheme.a graphics/libgraphics-x11.a
	${MAKE} -C customwm -f Makefile.x11 all DEBUG=${DEBUG}

#
# external dependencies
#

LUAW_PATH=contrib/luaw
LUAW_FILE=${LUAW_PATH}/README.md

${LUAW_FILE}:  # download luaw git submodule, if not there
	git submodule update --recursive --remote

luazh-jit: ${LUAW_FILE}
	$(MAKE) -C ${LUAW_PATH} $@
	cp ${LUAW_PATH}/$@ $@

libluaw-jit.a: ${LUAW_FILE}
	$(MAKE) -C ${LUAW_PATH} $@ DEBUG=${DEBUG}
	cp ${LUAW_PATH}/$@ $@

#
# clean
#

clean:
	${MAKE} -C theme clean DEBUG=${DEBUG}
	${MAKE} -C graphics -f Makefile.x11 clean DEBUG=${DEBUG}
	${MAKE} -C customwm -f Makefile.x11 clean DEBUG=${DEBUG}

distclean: clean
	rm -f *.a
	$(MAKE) -C ${LUAW_PATH} clean
