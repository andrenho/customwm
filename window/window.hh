#ifndef WINDOW_HH_
#define WINDOW_HH_

#include <memory>
#include "theme/types/types.hh"
#include "graphics/windowhandle.hh"
#include "graphics/pencil/pencil.hh"

class Window_ {
public:
    Window_(class Theme* theme, class Graphics* graphics, Rectangle const& rectangle);
    virtual ~Window_();

    virtual void draw() = 0;
    void expose(Rectangle const& rectangle);

    WindowHandle handle() const { return handle_; }

    Rectangle const &rectangle() const { return rectangle_; }

    static void create_lua_metatable(class Theme* theme);
    static constexpr const char* mt_identifier = "Window";

protected:
    class Theme* theme_;
    class Graphics* graphics_;

    WindowHandle handle_;
    Rectangle    rectangle_;
    std::unique_ptr<Pencil> pencil_;
};

#endif //WINDOW_HH_
