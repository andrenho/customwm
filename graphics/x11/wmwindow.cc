#include "wmwindow.hh"
#include "theme/logger.hh"

#include <X11/Xlib.h>

WM_Window::WM_Window(Display* dpy, Window parent_id, Window child_id, Rectangle const &rectangle)
        : parent_id(parent_id), child_id(child_id), rectangle(rectangle), dpy_(dpy)
{
    Visual* visual = DefaultVisual(dpy, DefaultScreen(dpy));
    colormap_ = XCreateColormap(dpy, parent_id, visual, AllocNone);
    gc_ = XCreateGC(dpy_, parent_id, 0, nullptr);
}

WM_Window::~WM_Window()
{
    XFreeGC(dpy_, gc_);

    std::vector<unsigned long> pixels(colors_.size());
    std::transform(colors_.begin(), colors_.end(), std::back_inserter(pixels),
                   [](std::pair<const Color, unsigned long>& p) { return p.second; });
    XFreeColors(dpy_, colormap_, pixels.data(), (int) pixels.size(), 0);

    XFreeColormap(dpy_, colormap_);
    XFlush(dpy_);
}

void WM_Window::fill(Color const &color)
{
    unsigned long pixel = get_color(color);
    XSetForeground(dpy_, gc_, pixel);
    XFillRectangle(dpy_, parent_id, gc_, 0, 0, rectangle.w, rectangle.h);
    XFlush(dpy_);
}

unsigned long WM_Window::get_color(Color const &color)
{
    // black or white?
    if (color.is_white())
        return WhitePixel(dpy_, DefaultScreen(dpy_));
    else if (color.is_black())
        return BlackPixel(dpy_, DefaultScreen(dpy_));

    // cached?
    auto it = colors_.find(color);
    if (it != colors_.end())
        return it->second;

    // non-cached
    XColor xcolor = {
            .red = static_cast<unsigned short>(color.r * 256),
            .green = static_cast<unsigned short>(color.g * 256),
            .blue = static_cast<unsigned short>(color.b * 256),
            .flags = DoRed | DoGreen | DoBlue,
    };
    if (XAllocColor(dpy_, colormap_, &xcolor) == 0)
        LOG.info("Color allocation failed (%d %d %d)", color.r, color.g, color.b);
    colors_.emplace(color, xcolor.pixel);
    return xcolor.pixel;
}
