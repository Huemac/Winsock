#include "PCH.hpp"
#include "WsaInit.hpp"
#include "LogWrap.hpp"

WsaInit::WsaInit() :
    m_status(WSAStartup(MAKEWORD(2, 2), &m_data))
{
    if (!Initialized())
    {
        CERR << "WSAStartup failed with error: " << m_status;
    }
}

WsaInit::~WsaInit()
{
    if (Initialized())
    {
        const int status = WSACleanup();

        if (status != 0)
        {
            CERR << "WSAStartup failed with error: " << status << '/' << WSAGetLastError();
        }
    }
}

bool WsaInit::Initialized() const
{
    return m_status == 0;
}

WsaInit::operator bool() const
{
    return Initialized();
}