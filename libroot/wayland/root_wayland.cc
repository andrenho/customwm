#include "root_wayland.hh"

RootWayland::RootWayland(std::optional<std::string> const& display)
    : server_(std::make_unique<ServerWayland>(display))
{

}

RootWayland::~RootWayland()
{
    server_.reset();
}
