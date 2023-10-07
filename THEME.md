## Properties

window.padding = Padding or function(inner_area) -> Padding
window.starting_pos = Point or function(inner_area) -> Point
window.on_expose(window)

## Window

window:area() = { x, y, w, h }
window:draw_rectangle(rect, color, fill?)
window:draw_rectangles({ rect... }, color, fill?)
window:draw_polygon({ point... }, color, fill?)
window:draw_line(point, point, color)
