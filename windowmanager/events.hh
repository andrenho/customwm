#ifndef EVENTS_HH_
#define EVENTS_HH_

class Events {
public:
    Events(class Theme* theme, class WindowManager* window_manager)
            : theme_(theme), window_manager_(window_manager) {}

    void init() {}

    void run();

private:
    class Theme* theme_;
    class WindowManager* window_manager_;
};

#endif //EVENTS_HH_
