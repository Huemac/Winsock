#pragma once

#include <WinSock2.h>

class AddressInfo
{
public:
    AddressInfo(const char* node, const char* service, const addrinfo& hints);
    ~AddressInfo();

    static AddressInfo ServerTcpAddressInfo(const char* node, const char* service);
    static AddressInfo ClientTcpAddressInfo(const char* node, const char* service);

    bool Initialized() const;
    operator bool() const;
    addrinfo* Get() const;

private:
    addrinfo* m_addressInfo = nullptr;
    const int m_status;
};