# v0.1 - Base X version

## Basic infra

- [x] Makefile, executable, load lua scripts
- [x] Create "libraries"
- [x] Run arguments
- [x] Logger

## Theme

- [x] Load theme, read props
- [x] Merge theme

## WM

- [x] Connect to X (X object)
- [x] Initialize WM
- [x] Events
- [x] New window - reparent, add existing windows
- [x] Window object
- [x] Resource manager
- [x] Fill, (RM: colors)
- [ ] Create releaseable version
  - [x] Build on Linux
  - [x] Auto get git submodules
  - [x] Package checklist
  - [x] Check for dependencies (CMake)
  - [x] Build instructions
  - [x] make install
  - [x] make dist
  - [ ] Create dummy version, run valgrind
  - [ ] PR template (checklist)
  - [ ] Autobuild in github
      - [ ] Compile all versions
      - [ ] Generate package
  - [ ] Create next steps
  - [ ] Reorganize TODO
- [ ] OpenGL version
- [ ] Wayland version
- [ ] Add existing windows
- [ ] Cursors
- [ ] X properties
- [ ] Hotspots
    - [ ] Click
    - [ ] Move (enter/leave)
    - [ ] Cursor on hotspot
- [ ] Focus management
    - [ ] Focus new windows
    - [ ] Roll back focus to previous window when window is closed
    - [ ] Change focus when window is clicked (and bring to front)
- [ ] Keyboard focus
- [ ] Close window
- [ ] Load theme
    - [ ] Super
- [ ] Grab / move / resize window + configure (keep window rect)
    - [ ] Move
    - [ ] Resize
    - [ ] Grabs and resizes are not perfect
    - [ ] Faster resize performance
    - [ ] Minimum size
    - [ ] Maximum screen location
- [ ] Maximize
    - [ ] Maximize
    - [ ] Restore
- [ ] Document classes in plantuml - maybe write a HACKING document (?)
- [ ] Focus redraw issues
- [ ] Improve window buffering
    - [ ] Draw to buffer, and then expose to window (?)
    - [ ] Use multiple windows (?)
- [ ] RM: Load image
- [ ] Display image
- [ ] RM: Load font
- [ ] Text
- [ ] ICCCM
    - [ ] Create XAtomManager
    - [ ] Minimize
    - Window manager properties
        - [ ] WM_STATE
        - [ ] Changing Window State
        - [ ] ConfigureWindow
        - [ ] ChangeWindowAttributes
    - Client properties
        - [ ] Parse WM_HINTS, WM_NORMAL_HINTS
        - [ ] WM_ICON_NAME, WM_ICON_SIZE (?)
        - [ ] WM_TRANSIENT_FOR (?)
        - [ ] WM_PROTOCOLS
            - [ ] WM_TAKE_FOCUS
            - [ ] WM_DELETE_WINDOW
        - [ ] WM_COLORMAP_WINDOWS (?)
        - [ ] WM_CLIENT_MACHINE (?)
    - [ ] Selections
        - https://specifications.freedesktop.org/clipboards-spec/clipboards-latest.txt
        - https://www.uninformativ.de/blog/postings/2017-04-02/0/POSTING-en.html
        - https://jameshunt.us/writings/managing-the-x11-clipboard/
    - [ ] Session Management
- [ ] EWMH
- [ ] Sample W95 theme

## Bar

## Menu

## Error management
- [x] luaw_to - check type
- [ ] Check for libraries
- [ ] Reload config file
- [ ] Better mouse button names
- [ ] Caching of text for better perfomance (maybe freetype?)
- [ ] lua_error management

---

# v0.2 - Base Wayland version

---

# Future
- Create new cursors
- Improved error handling using lua_pcall
- Virtual Desktops
- Key management



## Plan

- Version 0.1: basic reparenting window manager (+ compositing?)
- Version 0.2: ICCCM and EWMH
- Version 0.3: advanced configuration options
- Version 0.4: menu & bar
- Version 0.5: file manager
- Version 0.6: theme manager & user settings
- Version 0.7: vectors

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
* https://www.talisman.org/~erlkonig/misc/X-composite-tutorial/
* https://gitlab.freedesktop.org/xorg/app/xcompmgr/-/blob/master/xcompmgr.c?ref_type=heads&_sm_au_=iHVR3rZQP1n5DFq6L321jK0f1JH33
* https://stackoverflow.com/questions/54505795/request-image-from-X-compositing-wm-in-c-or-c

## Architecture

1. User provided theme file
2. customwm-theme (reads lua theme)
3. customwm
    1. Base theme - main logic
    2. XWindow Manager

The **XWindow Manager** has the following responsibilities:
- Own the outer windows and keep track of them
- Reparent and deal with inner window lifecycle
- WMEvents: either deal with them or send them up to the theme
- Execute things requested by the theme on X11

The **base theme** has the following responsabilities:
- Deal with mouse click, movements and keyboard shortcuts
- Apply configurations from the user provided theme
