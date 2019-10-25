#pragma once

#include "Socket.hpp"

class ServerSocket : public Socket
{
public:
    ServerSocket();
    ~ServerSocket();
    bool Bind(const addrinfo* info);
    bool Listen() const;
};