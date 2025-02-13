#pragma comment(lib, "Advapi32.lib")

#include "EventLog.h"

CEventLog::CEventLog(std::string szServiceName)
    : serviceName(std::move(szServiceName)), m_hEventSource(nullptr) {}

void CEventLog::SetServiceName(std::string szServiceName) {
    serviceName = std::move(szServiceName);
    m_hEventSource = nullptr;
}

CEventLog::~CEventLog() {
    if (m_hEventSource) {
        ::DeregisterEventSource(m_hEventSource);
    }
}

void CEventLog::LogEvent(WORD wType, DWORD dwID,
                         std::optional<std::string> pszS1,
                         std::optional<std::string> pszS2,
                         std::optional<std::string> pszS3) {
    DWORD lastErr = GetLastError();

    std::vector<const char*> eventMessages;
    if (pszS1) eventMessages.push_back(pszS1->c_str());
    if (pszS2) eventMessages.push_back(pszS2->c_str());
    if (pszS3) eventMessages.push_back(pszS3->c_str());

    // Ensure event source is registered
    if (!m_hEventSource) {
        m_hEventSource = ::RegisterEventSource(nullptr, serviceName.c_str());
    }

    if (m_hEventSource) {
        ::ReportEvent(m_hEventSource,
                      wType,
                      0,
                      dwID,
                      nullptr,
                      static_cast<WORD>(eventMessages.size()),
                      0,
                      eventMessages.data(),
                      nullptr);
    }

    SetLastError(lastErr);
}
