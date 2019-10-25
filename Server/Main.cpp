#include "PCH.hpp"
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "WsaInit.hpp"
#include "AddressInfo.hpp"
#include "LogWrap.hpp"

#pragma comment (lib, "Ws2_32.lib")

int wmain(int, wchar_t* [])
{
    WsaInit guard;

    if (!guard)
    {
        CERR << "Failed to initialize Winsock";
        return 1;
    }

    constexpr char service[] = "http";

    AddressInfo info = AddressInfo::ServerTcpAddressInfo(nullptr, service);

    if (!info)
    {
        CERR << "Failed to get address info for " << service;
        return 1;
    }

    ClientSocket client;

    {
        ServerSocket server;

        if (!server.Bind(info.Get()))
        {
            return -1;
        }

        if (!server.Listen())
        {
            return -2;
        }

        if (!client.Accept(server, SD_SEND))
        {
            return -3;
        }
    }

    while (client)
    {
        std::string data;

        if (!client.Receive(data))
        {
            CERR << "Failed to receive data from client!";
            return -4;
        }

        COUT << "Received: " << data;

        if (!client.Send("WELCOME"))
        {
            CERR << "Failed to send greeting to client!";
            return -5;
        }

        COUT << "Sent welcome";
    }

    return 0;
}