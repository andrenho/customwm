#include "waylandgraphics.hh"

#define MU [[maybe_unused]]

void WaylandGraphics::init()
{
    display = wl_display_create();
}

Size WaylandGraphics::screen_size() const
{
    return {0, 0};
}

std::vector<WindowHandle> WaylandGraphics::toplevel_windows() const
{
    return {};
}

WindowHandle WaylandGraphics::create_window(MU const Rectangle &rectangle)
{
    return 0;
}

void WaylandGraphics::destroy_window(MU WindowHandle window)
{

}

void WaylandGraphics::reparent_window(MU WindowHandle parent, MU WindowHandle child, MU const Point &offset)
{

}

void WaylandGraphics::unparent_window(MU WindowHandle child)
{

}

void WaylandGraphics::paint(MU Window_ *window, MU std::function<void()> paint_function)
{

}

Rectangle WaylandGraphics::get_window_rectangle(MU WindowHandle window) const
{
    return { 0,0,0,0 };
}

void WaylandGraphics::subscribe_to_wm_events()
{

}

std::optional<Event> WaylandGraphics::next_event()
{
    return {};
}

std::unique_ptr<Pencil> WaylandGraphics::create_pencil(MU Window_ *window)
{
    return nullptr;
    // return std::make_unique<OpenGLPencil>(window, this, &opengl_manager_);
}
