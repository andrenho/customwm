#ifndef GRAPHICS_HH_
#define GRAPHICS_HH_

class Graphics {
public:
    explicit Graphics(class Options* options) : options_(options) {}
    virtual ~Graphics() = default;

    virtual void init() = 0;

protected:
    class Options* options_;
};

#endif //GRAPHICS_HH_
