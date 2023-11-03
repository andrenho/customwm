# customwm
A highly customizable IWindow Manager for X.

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

## Plan

- Version 0.1: basic reparenting window manager (+ compositing?)
- Version 0.2: ICCCM and EWMH
- Version 0.3: advanced configuration options
- Version 0.4: menu & bar
- Version 0.5: file manager
- Version 0.6: theme manager & user settings
- Version 0.7: vectors

## Configurations

- window border/title images, fonts, behaviour
- initial window position
- desktop default background
- general behaviour: focus, click on desktop
- mouse pointers
- special keys
- workspaces
- user-configurable options
- dark/light mode
- bars

## ResourceManager

* https://magcius.github.io/xplain/article/x-basics.html
* https://github.com/mcpcpc/xwm
* https://jichu4n.com/posts/how-x-window-managers-work-and-how-to-write-one-part-i/
* https://community.kde.org/Xcb
* https://jichu4n.com/content/images/2018/10/sB-bXhvvzFJe2u65_YRwARA.png
* https://en.wikipedia.org/wiki/X_Window_System_protocols_and_architecture
* https://en.wikipedia.org/wiki/X_logical_font_description
* https://en.wikipedia.org/wiki/X_Window_System_core_protocol
* https://www.x.org/wiki/guide/
* https://www.x.org/releases/X11R7.5/doc/libxcb/tutorial/index.html
* https://github.com/QMonkey/Xlib-demo/blob/master/src/draw-image.c
* https://stackoverflow.com/questions/43029489/how-to-display-an-image-into-an-xcb-window

Fonts:

* https://gist.github.com/luser-dr00g/13b81dbf3d557a9a9671
* https://venam.nixers.net/blog/unix/2018/09/02/fonts-xcb.html
* https://xcb.freedesktop.org/tutorial/fonts/ ?

Composite:
* https://www.talisman.org/~erlkonig/misc/x11-composite-tutorial/
* https://gitlab.freedesktop.org/xorg/app/xcompmgr/-/blob/master/xcompmgr.c?ref_type=heads&_sm_au_=iHVR3rZQP1n5DFq6L321jK0f1JH33
* https://stackoverflow.com/questions/54505795/request-image-from-x11-compositing-wm-in-c-or-c

## Architecture

1. User provided theme file
2. customwm-theme (reads lua theme)
3. customwm
   1. Base theme - main logic
   2. XWindow Manager

The **XWindow Manager** has the following responsibilities:
 - Own the outer windows and keep track of them
 - Reparent and deal with inner window lifecycle
 - Events: either deal with them or send them up to the theme
 - Execute things requested by the theme on X11

The **base theme** has the following responsabilities:
 - Deal with mouse click, movements and keyboard shortcuts
 - Apply configurations from the user provided theme