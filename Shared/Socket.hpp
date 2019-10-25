#pragma once

#include <WinSock2.h>

class Socket
{
public:
    Socket();
    virtual ~Socket();

    operator SOCKET() const;
    bool Receive(std::string& data, unsigned short limit = 0x1000) const;

protected:
    bool Create(const addrinfo* info);
    void Close() const;

    SOCKET m_socket = INVALID_SOCKET;
    int m_closeFlags = SOCKET_ERROR;
};