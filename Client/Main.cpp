#include "PCH.hpp"
#include "WsaInit.hpp"
#include "AddressInfo.hpp"
#include "ClientSocket.hpp"
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

    constexpr char node[] = "127.0.0.1";
    constexpr char service[] = "http";

    AddressInfo info = AddressInfo::ClientTcpAddressInfo(nullptr, service);

    if (!info)
    {
        CERR << "Failed to get address info for " << service;
        return 1;
    }

    ClientSocket client;

    if (!client.Connect(info.Get()))
    {
        return -1;
    }

    while (client)
    {
        if (!client.Send("HELLO"))
        {
            return -2;
        }

        COUT << "Sent hello";

        std::string data;

        if (client.Receive(data))
        {
            COUT << "Received: " << data;
        }
    }

    return 0;
}