#include "PCH.hpp"
#include "ServerSocket.hpp"
#include "LogWrap.hpp"

ServerSocket::ServerSocket()
{
    CLOG;
}

ServerSocket::~ServerSocket()
{
    CLOG;
}

bool ServerSocket::Bind(const addrinfo* info)
{
    if (!Create(info))
    {
        CERR << "Failed to create socket!";
        return false;
    }

    const int result = bind(m_socket, info->ai_addr, static_cast<int>(info->ai_addrlen));

    if (result == SOCKET_ERROR)
    {
        CERR << "Failed to bind socket!";
        return false;
    }

    COUT << "Bound";
    return true;
}

bool ServerSocket::Listen() const
{
    if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        CERR << "Failed to listen socket!";
        return false;
    }

    COUT << "Listening...";
    return true;
}