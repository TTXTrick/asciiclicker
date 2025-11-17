#pragma once
#include <string>
#include <functional>

class NetClient {
public:
    bool online = false;
    std::function<void(std::string)> onMessage;

    virtual bool connect(const std::string& url) = 0;
    virtual void send(const std::string& msg) = 0;
    virtual void poll() = 0;
    virtual void close() = 0;
};
