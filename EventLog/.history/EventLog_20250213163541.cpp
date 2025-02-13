#pragma comment(lib, "Advapi32.lib")

#include "EventLog.h"

CEventLog::CEventLog(std::string szServiceName)
    : serviceName(std::move(szServiceName)), m_hEventSource(nullptr) {}

CEventLog::~CEventLog() {
    if (m_hEventSource) {
        ::DeregisterEventSource(m_hEventSource);
    }
}

void CEventLog::LogEvent(WORD wType, DWORD dwID, std::optional<std::string> pszS1, std::optional<std::string> pszS2, std::optional<std::string> pszS3) {
    if (!m_hEventSource) {
        m_hEventSource = ::RegisterEventSourceA(nullptr, serviceName.c_str());
        if (!m_hEventSource) {
            return;
        }
    }

    const char* pStrings[3] = {nullptr, nullptr, nullptr};
    int nStrings = 0;

    if (pszS1.has_value()) {
        pStrings[nStrings++] = pszS1.value().c_str();
    }

    if (pszS2.has_value()) {
        pStrings[nStrings++] = pszS2.value().c_str();
    }

    if (pszS3.has_value()) {
        pStrings[nStrings++] = pszS3.value().c_str();
    }

    ::ReportEventA(m_hEventSource, wType, 0, dwID, nullptr, nStrings, 0, pStrings, nullptr);
}