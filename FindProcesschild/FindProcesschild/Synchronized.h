#pragma once

class Synchronized
{
public:
    Synchronized(CRITICAL_SECTION& cs) : m_cs(cs) {
        ::EnterCriticalSection(&m_cs);
    }
    ~Synchronized() {
        ::LeaveCriticalSection(&m_cs);
    }

private:
    CRITICAL_SECTION& m_cs;
};
