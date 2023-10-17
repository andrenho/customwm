#ifndef METATABLES_HH_
#define METATABLES_HH_

struct RootUserData { class Root* root; };

void metatable_root_create(struct lua_State* L);

#endif //METATABLES_HH_
