#ifndef WINDOWMANAGER_HH_
#define WINDOWMANAGER_HH_

class WindowManager {
public:
    explicit WindowManager(class Theme* theme) : theme_(theme) {}

    void init() {}

private:
    class Theme* theme_;
};

#endif //WINDOWMANAGER_HH_
