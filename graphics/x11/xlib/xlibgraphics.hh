#ifndef XLIBGRAPHICS_HH_
#define XLIBGRAPHICS_HH_

#include "graphics/x11/xgraphics.hh"

class XlibGraphics : public XGraphics {
public:
    XlibGraphics(class Options *options, class Theme* theme) : XGraphics(options, theme) {}

    void paint(WindowHandle window, std::function<void()> paint_function) override;

    std::unique_ptr<Pencil> create_pencil(Window_* window) override;
};

#endif //XLIBGRAPHICS_HH_
