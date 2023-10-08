#ifndef RESOURCES_HH_
#define RESOURCES_HH_

#include <string>
#include <unordered_map>

#include <xcb/xcb.h>
#include "../libtheme/theme.hh"
#include "../libtheme/types/datafile.hh"
#include "../libtheme/types/types.hh"

class Resources {
public:
    Resources(xcb_connection_t *dpy, xcb_screen_t *scr) : dpy_(dpy), scr_(scr) {}
    ~Resources();

    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;
    Resources(Resources&&) = delete;
    Resources& operator=(Resources&&) = delete;

    void         load_resources(Theme const& theme);
    std::pair<xcb_pixmap_t, Rectangle> image(std::string const& image, std::string const& slice) const;

private:
    xcb_connection_t *dpy_;
    xcb_screen_t *scr_;

    struct Image {
        xcb_pixmap_t pixmap;
        std::unordered_map<std::string, Rectangle> slices;
    };
    std::unordered_map<std::string, Image> images_;

    void load_image(std::string const& name, DataFile const& df);
};

class ResourceNotFound : public std::runtime_error {
public:
    explicit ResourceNotFound(std::string const& what) : std::runtime_error(what) {}
};

#endif //RESOURCES_HH_
