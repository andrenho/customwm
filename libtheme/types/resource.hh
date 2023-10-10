#ifndef RESOURCE_HH_
#define RESOURCE_HH_

#include <string>

#include "../luaw.hh"
#include "types.hh"

#include <unordered_map>

struct ImageResource {
    enum { PNG } format;
    std::string data;
    std::unordered_map<std::string, Rectangle> slices;
};

template<> ImageResource luaw_to(lua_State* L, int index);

enum class FontType { X11 };

struct FontResource {
    FontType    format;
    std::string name;
};

template<> FontResource luaw_to(lua_State* L, int index);

#endif //RESOURCE_HH_
