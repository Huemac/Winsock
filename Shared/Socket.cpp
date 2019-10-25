#include "PCH.hpp"
#include "Socket.hpp"
#include "LogWrap.hpp"

Socket::Socket()
{
    CLOG;
}

Socket::~Socket()
{
    Close();
    CLOG;
}

Socket::operator SOCKET() const
{
    return m_socket;
}

bool Socket::Create(const addrinfo* info)
{
    Close();

    m_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

    if (m_socket == INVALID_SOCKET)
    {
        CERR << "Failed to initialize socket!";
        return false;
    }

    COUT << "Initialized";
    return true;
}

bool Socket::Receive(std::string& data, unsigned short limit) const
{
    Sleep(750); // TODO: remove

    std::string buffer(limit, '\0');

    const int received = recv(m_socket, &buffer.front(), limit, 0);

    if (received == SOCKET_ERROR)
    {
        CERR << "Failed to receive!";
        return false;
    }

    if (received == 0)
    {
        COUT << "Connection gracefully closed";
        return true;
    }

    if (received < 1)
    {
        COUT << "Nothing to receive: " << received;
        return false;
    }

    COUT << "Received " << received << " byte(s)";

    if (received < limit)
    {
        // Trim excess null
        buffer.resize(received);
    }

    data.append(buffer);
    return true;
}

void Socket::Close() const
{
    if (m_socket == INVALID_SOCKET)
    {
        // No need to close
        return;
    }

    if (m_closeFlags != SOCKET_ERROR && shutdown(m_socket, m_closeFlags) == SOCKET_ERROR)
    {
        CERR << "Socket shutdown failed with error:" << WSAGetLastError();
    }
    else
    {
        COUT << "Shutdown";
    }

    if (closesocket(m_socket) == SOCKET_ERROR)
    {
        CERR << "Socket closed with error:" << WSAGetLastError();
        return;
    }

    COUT << "Closed";
}