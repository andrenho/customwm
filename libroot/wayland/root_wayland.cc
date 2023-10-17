#include "root_wayland.hh"

RootWayland::RootWayland()
    : server_(std::make_unique<ServerWayland>())
{

}

RootWayland::~RootWayland()
{
    server_.reset();
}
