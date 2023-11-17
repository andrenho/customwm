#ifndef WINDOWMANAGER_HH_
#define WINDOWMANAGER_HH_

class WindowManager {
public:
    explicit WindowManager(class Theme* theme, class Graphics* graphics) : theme_(theme), graphics_(graphics) {}

    void init();

private:
    class Theme* theme_;
    class Graphics* graphics_;
};

#endif //WINDOWMANAGER_HH_
