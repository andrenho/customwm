#include "wmwindow.hh"

#include <X11/Xlib.h>

WM_Window::WM_Window(Display* dpy, Window parent_id, Window child_id, Rectangle const &rectangle)
        : parent_id(parent_id), child_id(child_id), rectangle(rectangle), dpy_(dpy)
{
    Visual* visual = DefaultVisual(dpy, DefaultScreen(dpy));
    colormap_ = XCreateColormap(dpy, parent_id, visual, AllocNone);
}

WM_Window::~WM_Window()
{
    XFreeColormap(dpy_, colormap_);
}