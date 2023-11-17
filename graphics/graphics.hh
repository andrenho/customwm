#ifndef GRAPHICS_HH_
#define GRAPHICS_HH_

class Graphics {
public:
    explicit Graphics(class Options* options) : options_(options) {}
    virtual ~Graphics() = default;

    virtual void init() = 0;

    virtual void listen_to_wm_events() = 0;

protected:
    class Options* options_;
};

#endif //GRAPHICS_HH_
