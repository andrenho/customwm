#ifndef RESOURCEMANAGER_HH_
#define RESOURCEMANAGER_HH_

#include <string>
#include <unordered_map>

#include <xcb/xcb.h>
#include "../libtheme/theme.hh"
#include "../libtheme/types/resources.hh"
#include "../libtheme/types/types.hh"

class ResourceManager {
public:
    struct Font {
        FontType type;
        union {
            xcb_font_t x11_font;
        };
    };

    ResourceManager(xcb_connection_t *dpy, xcb_screen_t *scr) : dpy_(dpy), scr_(scr) {}
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    void load_resources(Theme const& theme);

    std::pair<xcb_pixmap_t, Rectangle> image(std::string const& image, std::string const& slice) const;
    Font                               font(std::string const& name) const;

private:
    xcb_connection_t *dpy_;
    xcb_screen_t *scr_;

    struct Image {
        xcb_pixmap_t pixmap;
        xcb_pixmap_t mask_pixmap;
        std::unordered_map<std::string, Rectangle> slices;
    };
    std::unordered_map<std::string, Image> images_;
    std::unordered_map<std::string, Font> fonts_;

    void load_image(std::string const& name, ImageResource const& df);
    void load_font(std::string const &basic_string, FontResource const &resource);
};

class ResourceBroken : public std::runtime_error {
public:
    explicit ResourceBroken(std::string const& what) : std::runtime_error(what) {}
};

class ResourceNotFound : public std::runtime_error {
public:
    explicit ResourceNotFound(std::string const& what) : std::runtime_error(what) {}
};

#endif //RESOURCEMANAGER_HH_