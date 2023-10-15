#ifndef SERVER_HH_
#define SERVER_HH_

class Server {
public:
    virtual ~Server() = default;

    virtual void setup_event_listeners() = 0;
    virtual void capture_existing_windows() = 0;
    virtual void run_event_loop() = 0;
};

#endif //SERVER_HH_
