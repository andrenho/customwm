#include "theme.hh"

Theme::Theme()
    : L(luaL_newstate())
{
    luaL_openlibs(L);
}

Theme::~Theme()
{
    lua_close(L);
}

void Theme::load_from_ram(const unsigned char *data, unsigned int len)
{

}
