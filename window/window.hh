#ifndef WINDOW_HH_
#define WINDOW_HH_

#include "theme/types/types.hh"
#include "graphics/windowhandle.hh"

class Window_ {
public:
    Window_(class Theme* theme, class Graphics* graphics, Rectangle const& rectangle);
    virtual ~Window_();

    void draw();

    WindowHandle handle() const { return handle_; }

    static void create_lua_metatable(class Theme* theme);
    static constexpr const char* mt_identifier = "Window";

protected:
    class Theme* theme_;
    class Graphics* graphics_;

    WindowHandle handle_;
    Rectangle    rectangle_;

private:
    void fill(Color const& color, std::optional<Rectangle> rect);
};

#endif //WINDOW_HH_
