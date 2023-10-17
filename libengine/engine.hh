#ifndef ENGINE_HH_
#define ENGINE_HH_

#include <string>
#include "../libroot/root.hh"

class Engine {
public:
    Engine();
    ~Engine();

    void load_from_memory(std::string const& name, uint8_t *data, unsigned int sz);
    void override_with_theme(std::string const& filename);

    void setup_root_object(class Root* root);

    template<typename... Types> void call(std::string const& prop_name, Types&... args);
    template<typename... Types> void call_opt(std::string const& prop_name, Types&... args);

private:
    struct lua_State *L = nullptr;
};

#include "engine.inl"

#endif //ENGINE_HH_
