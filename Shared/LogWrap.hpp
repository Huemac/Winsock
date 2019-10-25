#pragma once

#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

class LogWrap
{
public:
    template<std::size_t N>
    LogWrap(std::ostream& stream, const char(&function)[N], int line) :
        m_stream(stream)
    {
        const auto now = std::chrono::system_clock::now();
        const auto tt = std::chrono::system_clock::to_time_t(now);
        m_buffer << '[' << tt << "][" << function << ':' << line << "] ";
    }

    inline ~LogWrap()
    {
        std::cout << m_buffer.str() << std::endl;
    }

    template<typename T>
    LogWrap& operator << (const T& value)
    {
        m_buffer << value;
        return *this;
    }

    template<std::size_t N>
    LogWrap& operator << (const char(&value)[N])
    {
        m_buffer << value;
        return *this;
    }

private:
    const std::ostream& m_stream;
    std::stringstream m_buffer;
};

#define CLOG LogWrap(std::clog, __FUNCTION__, __LINE__)
#define COUT LogWrap(std::cout, __FUNCTION__, __LINE__)
#define CERR LogWrap(std::cerr, __FUNCTION__, __LINE__)