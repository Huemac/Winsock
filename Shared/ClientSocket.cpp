#include "PCH.hpp"
#include "ClientSocket.hpp"
#include "LogWrap.hpp"

ClientSocket::ClientSocket()
{
    CLOG;
}

ClientSocket::~ClientSocket()
{
    CLOG;
}

bool ClientSocket::Connect(const addrinfo* info)
{
    for (const addrinfo* iter = info; iter != nullptr; iter = iter->ai_next)
    {
        if (!Create(iter))
        {
            CERR << "Failed to create socket!";
            return false;
        }

        int result = connect(m_socket, iter->ai_addr, static_cast<int>(iter->ai_addrlen));

        if (result == SOCKET_ERROR)
        {
            Close();
            continue;
        }

        COUT << "Connected";
        return true;
    }

    CERR << "Failed to connect!";
    return false;
}

bool ClientSocket::Send(const std::string& data) const
{
    Sleep(250); // TODO: remove

    if (data.length() >= INT_MAX)
    {
        CERR << "Failed to send! Payload too big. Size=" << data.length();
        return false;
    }

    if (send(m_socket, data.c_str(), static_cast<int>(data.length()), 0) == SOCKET_ERROR)
    {
        CERR << "Failed to send!";
        return false;
    }

    COUT << "Sent " << data.length() << " byte(s)";
    return true;
}

bool ClientSocket::Accept(SOCKET socket, int closeFlags)
{
    Close();

    m_socket = accept(socket, nullptr, nullptr);

    if (m_socket == INVALID_SOCKET)
    {
        CERR << "Failed to accept socket!";
        return false;
    }

    m_closeFlags = closeFlags;
    COUT << "Accepted";
    return true;
}