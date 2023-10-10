#include "resource.hh"
#include "../exceptions.hh"

#include <fstream>
#include <sstream>
#include <string>

static std::string base64_decode(const std::string &in) {

    std::string out;
    out.reserve(8 * (1 + in.size() / 6));

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val=0, valb=-8;
    for (uint8_t c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

template<> ImageResource luaw_to(lua_State* L, int index)
{
    int top = lua_gettop(L);

    ImageResource df;

    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, -1, "format");
    if (lua_type(L, -1) != LUA_TSTRING)
        throw LuaException(L, "The image format was not specified.");
    std::string fmt = lua_tostring(L, -1);
    if (fmt == "png")
        df.format = ImageResource::PNG;
    else
        throw LuaException(L, "invalid image format `" + fmt + "`");
    lua_pop(L, 1);

    bool loaded = false;

    lua_getfield(L, -1, "base64");
    if (!lua_isnil(L, -1)) {
        df.data = base64_decode(lua_tostring(L, -1));
        loaded = true;
    }
    lua_pop(L, 1);

    lua_getfield(L, -1, "file");
    if (!lua_isnil(L, -1)) {
        std::ifstream f(lua_tostring(L, -1));
        std::stringstream buffer;
        buffer << f.rdbuf();
        df.data = buffer.str();
        loaded = true;
    }
    lua_pop(L, 1);

    if (!loaded)
        throw LuaException(L, "No images where presented to load. Use either `base64` or `file`");

    lua_getfield(L, -1, "slices");
    if (!lua_isnil(L, -1)) {
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            df.slices.emplace(lua_tostring(L, -2), luaw_to<Rectangle>(L, -1));
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);

    luaw_asserttop(L, top);

    return df;
}


template<> FontResource luaw_to(lua_State* L, int index)
{
    int top = lua_gettop(L);

    FontResource fr;

    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, -1, "format");
    if (lua_type(L, -1) != LUA_TSTRING)
        throw LuaException(L, "The font format was not specified.");
    std::string fmt = lua_tostring(L, -1);
    if (fmt == "x11")
        fr.format = FontType::X11;
    else
        throw LuaException(L, "invalid font format `" + fmt + "`");
    lua_pop(L, 1);

    lua_getfield(L, -1, "name");
    if (lua_isnil(L, -1))
        throw LuaException(L, "The font name was not specified");
    fr.name = luaL_checkstring(L, -1);

    lua_pop(L, 1);

    luaw_asserttop(L, top);

    return fr;
}