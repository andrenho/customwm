## Theme

Window:
  - [x] Read window name
  - [x] Set correct cursor
  - [x] Define hotspot areas
  - [x] Move window
    - [x] On click (clicks going through?)
    - [x] On hotspot click (?)
    - [x] Attach window to pointer
    - [x] On move
  - [ ] Window focus
  - [ ] Grab
    - [ ] Set cursor
  - [ ] Resize
  - [ ] Minimize
  - [ ] Maximize

W95 theme:
  - [ ] Graphics
    - [ ] Code to position graphics on window

Menu

Bar

Improvements:
  - [x] Global Visual, Screen, Colormap, etc...
  - [x] Create X window in WMWindow
  - [x] Strategic flushes
  - [x] Set child as property of id
      - [x] Move properties to resources
      - [x] Set XWindow ATOM
  - [x] Do some organization on WM / XWindow
  - [x] Move cursor to resources
  - [x] Update cursor from theme
  - [x] On enter/on leave hotspot
    - [x] Fix hotspot when leaving window
  - [x] Bug - start application with existing window
  - [ ] Cache properties
  - [ ] Deal with errors
    - [ ] Throw first, and then log
    - [ ] Better errors in luaw

Error management
  - [x] luaw_to - check type
  - [ ] Check for libraries
  - [ ] Reload config file
  - [ ] Better mouse button names
  - [ ] lua_error management

Future:
  - Create new cursors