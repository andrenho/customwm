# customwm

**customwm is currently in alpha, and not at a stage that is considered usable.**
That being said, it should be compilable and - what is developed - should be functional.

---

A highly customizable Window Manager for X.

The WindowList allows for applying themes on the fly. Each theme lives in a zip that contains a Lua file with the configuration, along with the resource files.

The programs are:

- **customwm**: the X window manager
- **customwm-bar**: a highly configurable bar
- **customwm-menu**: a highly configurable menu
- **customwm-fm**: a highly configurable file manager
- **customwm-theme-parser**: parse a theme file, and from that generates config files for GTK, KDE, etc...
- **customwm-themes**: a theme viewer and switcher
- **customwm-settings**: set X11 + theme settings
- **customwm-lib**: a library used by all, allows reading config files and has a layer on top of X


## Building

```shell
mkdir build
cd build
cmake ..
make
sudo make install
```

### Alternative build options

By default, the application will be built with both X11 and Wayland support.

To build for X11 only (no OpenGL):

```shell
cmake -DWITH_OPENGL=OFF -DWITH_WAYLAND=OFF ..
```

To build for X11 only (with OpenGL):

```shell
cmake -DWITH_WAYLAND=OFF ..
```

To build for Wayland only:

```shell
cmake -DWITH_X11=OFF ..
```
