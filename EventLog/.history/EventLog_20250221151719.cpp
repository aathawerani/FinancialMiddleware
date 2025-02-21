#include "EventLog.h"
#include <iostream>

#pragma comment(lib, "Advapi32.lib")

CEventLog::CEventLog(std::string_view szServiceName)
    : serviceName(szServiceName), m_hEventSource(nullptr) {}

CEventLog::~CEventLog() {
    if (m_hEventSource) {
        ::DeregisterEventSource(m_hEventSource);
    }
}

void CEventLog::SetServiceName(std::string_view szServiceName) {
    serviceName = szServiceName;
}

void CEventLog::LogEvent(WORD wType, DWORD dwID,
                         std::optional<std::string_view> pszS1,
                         std::optional<std::string_view> pszS2,
                         std::optional<std::string_view> pszS3) {
    if (!m_hEventSource) {
        m_hEventSource = ::RegisterEventSourceA(nullptr, serviceName.c_str());
        if (!m_hEventSource) {
            std::cerr << "Failed to register event source." << std::endl;
            return;
        }
    }

    std::vector<const char*> messages;
    if (pszS1) messages.push_back(pszS1->data());
    if (pszS2) messages.push_back(pszS2->data());
    if (pszS3) messages.push_back(pszS3->data());

    ::ReportEventA(m_hEventSource, wType, 0, dwID, nullptr,
                   static_cast<WORD>(messages.size()), 0,
                   messages.data(), nullptr);
}
