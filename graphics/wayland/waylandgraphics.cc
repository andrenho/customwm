#include "waylandgraphics.hh"

extern "C" {
#include <wlr/util/log.h>
#include <wlr/render/allocator.h>
#include <wlr/backend/multi.h>
}

#include "util/log.hh"

#define MU [[maybe_unused]]

void WaylandGraphics::init()
{
    if (options_->debug_mode)
        wlr_log_init(WLR_DEBUG, nullptr);

    display = wl_display_create();
    if (!display) {
        error("Could not create a wayland display");
        exit(EXIT_FAILURE);
    }

    backend = wlr_backend_autocreate(display, nullptr);
    if (!wlr_backend_start(backend)) {
        error("Could not start wlroots backend");
        exit(EXIT_FAILURE);
    }

    renderer = wlr_renderer_autocreate(backend);
    if (!renderer) {
        error("Failed to create renderer");
        exit(EXIT_FAILURE);
    }
    wlr_renderer_init_wl_display(renderer, display);

    event_loop = wl_display_get_event_loop(display);
}

std::optional<Event> WaylandGraphics::next_event()
{
    wl_display_run(display);

    return {};
}


WaylandGraphics::~WaylandGraphics()
{
    // TODO - cleanup
    wl_display_destroy(display);
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

std::unique_ptr<Pencil> WaylandGraphics::create_pencil(MU Window_ *window)
{
    return nullptr;
    // return std::make_unique<OpenGLPencil>(window, this, &opengl_manager_);
}

