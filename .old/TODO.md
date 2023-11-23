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
- [x] X properties
- [x] Events
- [x] New window - reparent, add existing windows
- [x] Window object
- [x] Resource manager
- [x] Fill, (RM: colors)
- [x] RM: Load image
- [x] Display image
- [x] RM: Load font
- [x] Text
- [x] Add existing windows
- [x] Cursors
- [x] Hotspots
  - [x] Click
  - [x] Move (enter/leave)
  - [x] Cursor on hotspot
- [x] Focus management
  - [x] Focus new windows
  - [x] Roll back focus to previous window when window is closed
  - [x] Change focus when window is clicked (and bring to front)
- [x] Keyboard focus
- [x] Close window
- [x] Load theme
  - [x] Super
- [x] Grab / move / resize window + configure (keep window rect)
  - [x] Move
  - [x] Resize
  - [x] Grabs and resizes are not perfect
  - [x] Faster resize performance
  - [x] Minimum size
  - [x] Maximum screen location
- [x] Maximize
  - [x] Maximize
  - [x] Restore
- [x] Document classes in plantuml - maybe write a HACKING document (?)
- [x] Focus redraw issues
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