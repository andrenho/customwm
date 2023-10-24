#include "graphicsx11.hh"
#include "wmx11.hh"

GraphicsX11::GraphicsX11(std::optional<std::string> display)
{

}

std::unique_ptr<WM> GraphicsX11::create_wm_()
{
    return std::make_unique<WMX11>();
}
