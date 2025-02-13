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

    std::vector<std::wstring> eventMessages;
    if (pszS1) eventMessages.push_back(std::wstring(pszS1->begin(), pszS1->end()));
    if (pszS2) eventMessages.push_back(std::wstring(pszS2->begin(), pszS2->end()));
    if (pszS3) eventMessages.push_back(std::wstring(pszS3->begin(), pszS3->end()));
    std::vector<LPCWSTR> eventMessagesCStr;
    for (const auto& msg : eventMessages) {
        eventMessagesCStr.push_back(msg.c_str());
    }

    // Ensure event source is registered
    if (!m_hEventSource) {
        std::wstring wServiceName(serviceName.begin(), serviceName.end());
        m_hEventSource = ::RegisterEventSource(nullptr, wServiceName.c_str());
    }

    if (m_hEventSource) {
        ::ReportEvent(m_hEventSource,
                      wType,
                      0,
                      dwID,
                      nullptr,
                      static_cast<WORD>(eventMessages.size()),
                      0,
                      eventMessagesCStr.data(),
                      nullptr);
    }

    SetLastError(lastErr);
}
