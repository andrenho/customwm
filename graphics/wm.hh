#ifndef WM_HH_
#define WM_HH_

class WM {
public:
    virtual ~WM() = default;

    virtual void run() = 0;
};

#endif //WM_HH_
