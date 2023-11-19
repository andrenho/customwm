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
- [ ] Fill, (RM: colors)
- [ ] RM: Load image
- [ ] Display image
- [ ] RM: Load font
- [ ] Text
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
