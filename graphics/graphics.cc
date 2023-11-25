#include "graphics.hh"
#include "graphics/x11/xgraphics.hh"
#include "graphics/dummy/dgraphics.hh"
#include "graphics/x11/gl/xopenglgraphics.hh"

std::unique_ptr<Graphics> Graphics::create_unique_ptr(Options* options, class Theme* theme)
{
    switch (options->backend) {
        case Options::Backend::Dummy:
            return std::make_unique<DGraphics>(options);
        case Options::Backend::X11:
            if (options->render == Options::Render::OpenGL)
                return std::make_unique<XOpenGLGraphics>(options, theme);
            else
                return std::make_unique<XGraphics>(options, theme);
        case Options::Backend::Wayland:
            throw std::runtime_error("sorry: wayland not supported yet");
    }
    throw std::runtime_error("Invalid option");
}
