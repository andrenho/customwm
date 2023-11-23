#include "graphics.hh"
#include "graphics/x11/xgraphics.hh"
#include "graphics/dummy/dgraphics.hh"

std::unique_ptr<Graphics> Graphics::create_unique_ptr(Options* options, class Theme* theme)
{
    switch (options->backend) {
        case Options::Backend::Dummy:
            return std::make_unique<DGraphics>(options);
        case Options::Backend::X11:
            return std::make_unique<XGraphics>(options, theme);
        case Options::Backend::Wayland:
            throw std::runtime_error("sorry: wayland not supported yet");
    }
}
