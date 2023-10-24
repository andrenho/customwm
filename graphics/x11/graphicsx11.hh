#ifndef GRAPHICSX11_HH_
#define GRAPHICSX11_HH_

#include "../graphics.hh"

#include <optional>
#include <string>

class GraphicsX11 : public Graphics {
public:
    explicit GraphicsX11(std::optional<std::string> display);

    std::unique_ptr<WM> create_wm_() override;
};

#endif //GRAPHICSX11_HH_
