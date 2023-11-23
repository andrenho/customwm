#!/usr/bin/env bash

VERSION=$(<PRJ_VERSION)

make -C contrib/luaw distclean
git clean -fdx
git submodule update --init --remote --recursive
mkdir customwm-$VERSION
rsync -lrv --exclude=.git --exclude=.old --exclude=customwm-$VERSION --exclude=*.o --exclude=.gitignore --exclude=.gitmodules --exclude=.idea --exclude=*.a --exclude=*.so --exclude=luazh-jit.dSYM --exclude=.github . customwm-$VERSION
tar -cvjSf customwm-$VERSION.tar.bz2 customwm-$VERSION
rm -rf customwm-$VERSION