#include "resourcesx11.hh"


// see: https://archive.irixnet.org/apocrypha/nekonomicon/forum/15/8794/1.html

ResourcesX11::~ResourcesX11()
{

}

XftFont* ResourcesX11::get_font(std::string const& name)
{
    auto it = fonts_.find(name);
    if (it != fonts_.end())
        return it->second;

    XftFont* font = XftFontOpenName(dpy_, DefaultScreen(dpy_), name.c_str());

    return nullptr;
}
