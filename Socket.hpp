#pragma once

#include <winsock2.h>
#include <string>

class Socket
{
public:
    Socket();
    ~Socket();

    operator SOCKET() const;
    void operator = (const addrinfo* info);

    bool Bind(const addrinfo* info);
    bool Accept(SOCKET socket, int closeFlags);
    
    bool Listen() const;
    bool Send(const std::string& data) const;
    bool Receive(std::string& data, unsigned short limit = 0x1000) const;

private:
    void Close() const;

    SOCKET m_socket = INVALID_SOCKET;
    int m_closeFlags = SOCKET_ERROR;
};
