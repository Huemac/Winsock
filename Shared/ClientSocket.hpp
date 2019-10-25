#pragma once

#include "Socket.hpp"

class ClientSocket : public Socket
{
public:
    ClientSocket();
    ~ClientSocket();

    bool Connect(const addrinfo* info);
    bool Send(const std::string& data) const;
    bool Accept(SOCKET socket, int closeFlags);
};