## Properties

metadata = { name, author, link={} }
window.padding = Padding or function(inner_area) -> Padding
window.starting_pos = Point or function(inner_area) -> Point
window.on_expose(window)
images.* = { format="png", base64="" or file="", slices = { * = { x, y, w, h } } }

## Window

window:area() = { x, y, w, h }
window:draw_rectangle(rect, color, fill?)
window:draw_rectangles({ rect... }, color, fill?)
window:draw_polygon({ point... }, color, fill?)
window:draw_line(point, point, color)
