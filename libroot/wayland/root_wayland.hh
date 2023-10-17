#ifndef ROOTX11_HH_
#define ROOTX11_HH_

#include "../root.hh"
#include "server_wayland.hh"

class RootWayland : public Root {
public:
    explicit RootWayland(Engine& engine);
    ~RootWayland() override;

    std::string interface_name() const override { return "wayland"; }
    Server &server() override { return *server_; }

private:
    std::unique_ptr<ServerWayland> server_;
};

#endif //ROOTX11_HH_
