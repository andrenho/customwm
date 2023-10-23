#ifndef SERVER_HH_
#define SERVER_HH_

class Server {
public:
    virtual ~Server() = default;

    virtual void run() = 0;

protected:
    explicit Server(class Engine& engine) : engine_(engine) {}

    class Engine& engine_;
};

#endif //SERVER_HH_
