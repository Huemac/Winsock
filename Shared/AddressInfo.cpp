#include "PCH.hpp"
#include "AddressInfo.hpp"

AddressInfo::AddressInfo(const char* node, const char* service, const addrinfo& hints) :
    m_status(getaddrinfo(node, service, &hints, &m_addressInfo))
{
    if (m_status != 0)
    {
        wprintf_s(L"Getaddrinfo failed with error: %d / %d\n", m_status, WSAGetLastError());
    }
}

AddressInfo::~AddressInfo()
{
    if (Initialized())
    {
        freeaddrinfo(m_addressInfo);
    }
}

AddressInfo AddressInfo::ServerTcpAddressInfo(const char* node, const char* service)
{
    constexpr addrinfo hints =
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

    return AddressInfo(node, service, hints);
}

AddressInfo AddressInfo::ClientTcpAddressInfo(const char* node, const char* service)
{
    constexpr addrinfo hints =
    {
        0, // ai_flags
        AF_INET, // ai_family
        SOCK_STREAM, // ai_socktype
        IPPROTO_TCP, // ai_protocol
        size_t(0), // ai_addrlen
        nullptr, // ai_canonname
        nullptr, // ai_addr
        nullptr, // ai_next
    };

    return AddressInfo(node, service, hints);
}

bool AddressInfo::Initialized() const
{
    return m_status == 0;
}

AddressInfo::operator bool() const
{
    return Initialized();
}

addrinfo* AddressInfo::Get() const
{
    return m_addressInfo;
}