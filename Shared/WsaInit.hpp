#pragma once

#include <WinSock2.h>

class WsaInit
{
public:
    WsaInit();
    ~WsaInit();
    bool Initialized() const;
    operator bool() const;

private:
    WSADATA m_data = { 0 };
    const int m_status;
};