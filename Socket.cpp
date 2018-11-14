#include "Socket.hpp"

#include <stdio.h>

Socket::Socket()
{
    _putws(L"Dummy socket created");
}

Socket::~Socket()
{
    Close();
}

Socket::operator SOCKET() const
{
    return m_socket;
}

void Socket::operator = (const addrinfo* info)
{
    Close();

    m_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

    if (m_socket == INVALID_SOCKET)
    {
        _putws(L"Failed to initialize socket!");
        return;
    }

    _putws(L"Socket initialized");
}

bool Socket::Bind(const addrinfo* info)
{
    const int result = bind(m_socket, info->ai_addr, static_cast<int>(info->ai_addrlen));

    if (result == SOCKET_ERROR)
    {
        _putws(L"Failed to bind socket!");
        return false;
    }

    _putws(L"Socket bound");
    return true;
}

bool Socket::Listen() const
{
    if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        _putws(L"Failed to listen socket!");
        return false;
    }

    _putws(L"Socket listening...");
    return true;
}

bool Socket::Accept(SOCKET socket, int closeFlags)
{
    Close();

    m_socket = accept(socket, nullptr, nullptr);

    if (m_socket == INVALID_SOCKET)
    {
        _putws(L"Failed to accept socket!");
        return false;
    }

    m_closeFlags = closeFlags;
    _putws(L"Socked accepted");
    return true;
}

bool Socket::Send(const std::string& data) const
{
    if (data.length() >= INT_MAX)
    {
        wprintf_s(L"Failed to send! Payload too big: %zu byte(s)\n", data.length());
        return false;
    }

    if (send(m_socket, data.c_str(), static_cast<int>(data.length()), 0) == SOCKET_ERROR)
    {
        _putws(L"Failed to send!");
        return false;
    }

    wprintf_s(L"Sent %zu byte(s)\n", data.length());
    return true;
}

bool Socket::Receive(std::string& data, unsigned short limit) const
{
    std::string buffer(limit, '\0');

    const int received = recv(m_socket, &buffer.front(), limit, 0);

    if (received == SOCKET_ERROR)
    {
        _putws(L"Failed to receive!");
        return false;
    }

    if (received == 0)
    {
        _putws(L"Connection gracefully closed");
        return true;
    }

    if (received < 1)
    {
        wprintf_s(L"Nothing to receive: '%d'\n", received);
        return false;
    }

    wprintf_s(L"Received %d byte(s)\n", received);

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
        wprintf_s(L"Socket shutdown failed with error: %d\n", WSAGetLastError());
    }
    else
    {
        _putws(L"Socket shutdown");
    }

    if (closesocket(m_socket) == SOCKET_ERROR)
    {
        wprintf_s(L"Socket closed with error: %d\n", WSAGetLastError());
        return;
    }

    _putws(L"Socket closed");
}