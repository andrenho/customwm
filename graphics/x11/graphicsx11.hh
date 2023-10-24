#ifndef GRAPHICSX11_HH_
#define GRAPHICSX11_HH_

#include "../graphics.hh"

#include <optional>
#include <string>

#include <xcb/xcb.h>
#include <xcb/xcb_errors.h>

class GraphicsX11 : public Graphics {
public:
    explicit GraphicsX11(std::optional<std::string> display);

    std::unique_ptr<WM> create_wm_() override;

private:
    xcb_connection_t*     dpy_ = nullptr;
    xcb_screen_t*         scr_ = nullptr;
    xcb_errors_context_t* err_ctx = nullptr;
};

#endif //GRAPHICSX11_HH_
