#ifndef WM_HH_
#define WM_HH_


#include "../lib/customwmlib.hh"
#include "windows.hh"

class WM {
public:
    explicit WM(CustomWMLib const& lib) : lib_(lib) {}

    void run();

protected:
    virtual void setup() = 0;
    virtual void loop() = 0;

    CustomWMLib const& lib_;
    Windows windows_;
};

#endif //WM_HH_
