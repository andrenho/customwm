#include "root_wayland.hh"

RootWayland::RootWayland(Engine& engine)
    : Root(engine), server_(std::make_unique<ServerWayland>())
{

}

RootWayland::~RootWayland()
{
    server_.reset();
}
