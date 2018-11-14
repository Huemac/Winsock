#define WIN32_LEAN_AND_MEAN

#if _DEBUG
// #include <vld.h>
#endif

// Windows
#include <windows.h>
#include <Winsock2.h>
#include <ws2tcpip.h>

// STL
#include <memory>
#include <regex>
#include <cstdlib>
#include <cstdio>

// Solution
#include "Socket.hpp"

#pragma comment (lib, "Ws2_32.lib")

namespace
{
    std::shared_ptr<WSADATA> InitalizeWinsock()
    {
        auto buffer = new WSADATA();
        const int status = WSAStartup(MAKEWORD(2, 2), buffer);
        std::shared_ptr<WSADATA> result(buffer, [](WSADATA* data)
        {
            delete data;

            const int status = WSACleanup();

            if (status != 0)
            {
                wprintf_s(L"WSACleanup failed with error: %d / %d\n", status, WSAGetLastError());
            }
        });

        if (status != 0)
        {
            wprintf_s(L"WSAStartup failed with error: %d\n", status);
            return nullptr;
        }

        return result;
    }

    // Resolve the server address and port
    std::shared_ptr<addrinfo> InitializeAddressInfo(const char* node, const char* service, const addrinfo& hints)
    {
        addrinfo* buffer = nullptr;
        const int status = getaddrinfo(node, service, &hints, &buffer);
        std::shared_ptr<addrinfo> result(buffer, freeaddrinfo);

        if (status != 0)
        {
            wprintf_s(L"Getaddrinfo failed with error: %d / %d\n", status, WSAGetLastError());
            return nullptr;
        }

        return result;
    }
}

int wmain(int, wchar_t*[])
{
    const auto guard = InitalizeWinsock();

    if (!guard)
    {
        _putws(L"Failed to initialize Winsock");
        return 1;
    }

    Socket client;
    {
        Socket server;
        {
            const char service[] = "http";
            const addrinfo hints =
            {
                AI_PASSIVE, // ai_flags
                AF_INET, // ai_family
                SOCK_STREAM, // ai_socktype
                IPPROTO_TCP, // ai_protocol
                size_t(0), // ai_addrlen
                nullptr, // ai_canonname
                nullptr, // ai_addr
                nullptr, // ai_next
            };

            auto info = InitializeAddressInfo(nullptr, service, hints);

            if (!info)
            {
                wprintf_s(L"Failed to get address info for %hs\n", service);
                return 1;
            }

            // Create server socket
            server = info.get();

            if (server == INVALID_SOCKET)
            {
                wprintf_s(L"Failed to open socket with error: %d\n", WSAGetLastError());
                return 1;
            }

            // Setup the TCP listening socket
            if (!server.Bind(info.get()))
            {
                wprintf_s(L"Bind failed with error: %d\n", WSAGetLastError());
                return 1;
            }
        }

        // Start listening for connections
        if (!server.Listen())
        {
            wprintf_s(L"Listen failed with error: %d\n", WSAGetLastError());
            return 1;
        }

        _putws(L"Waiting for connection...");

        // Open browser and navigate to http://localhost/
        if (!client.Accept(server, SD_SEND))
        {
            wprintf_s(L"Accept failed with error: %d\n", WSAGetLastError());
            return 1;
        }

        _putws(L"A client connected!");
    }

    // Receveive GET headers
    std::string request;

    if (!client.Receive(request))
    {
        wprintf_s(L"Receive failed with error: %d\n", WSAGetLastError());
    }

    std::string message = u8R"(<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Hello World!</title>
  </head>
  <body>
    <h1>Hello World!</h1>
    <p>Greetings from C++11</p>
    <p>Your request headers:</p>
    <pre></pre>
  </body>
</html>)";

    // Inject headers between <pre></pre> tags
    message = std::regex_replace(message, std::regex("(<pre>)"), "$1" + request);

    // Send Hello to the connected client
    if (!client.Send(message))
    {
        wprintf_s(L"Send failed with error: %d\n", WSAGetLastError());
    }

    _putws(L"Connection closing...");

    return 0;
}