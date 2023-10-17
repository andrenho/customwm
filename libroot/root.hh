#ifndef ROOT_HH_
#define ROOT_HH_

#include <memory>
#include <optional>
#include <string>

#include "server.hh"

class Root {
public:
    static std::unique_ptr<Root> build(std::optional<std::string> const& display, class Engine& engine);

    virtual ~Root() = default;

    virtual std::string interface_name() const = 0;
    virtual Server& server() = 0;

protected:
    Root(class Engine& engine) : engine_(engine) {}

    class Engine& engine_;
};

#endif //ROOT_HH_
