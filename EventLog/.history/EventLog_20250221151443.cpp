#include <windows.h>
#include <string>
#include <array>
#include <vector>
#include <optional>
#include <span>
#include <format>

#pragma comment(lib, "Advapi32.lib")

class CEventLog {
public:
    explicit CEventLog(std::string szServiceName);
    void SetServiceName(std::string szServiceName);
    ~CEventLog();

    void LogEvent(WORD wType, DWORD dwID, 
                  std::optional<std::string> pszS1 = std::nullopt,
                  std::optional<std::string> pszS2 = std::nullopt,
                  std::optional<std::string> pszS3 = std::nullopt);

private:
    HANDLE m_hEventSource = nullptr;
    std::string serviceName;
};

// Constructor
CEventLog::CEventLog(std::string szServiceName)
    : serviceName(std::move(szServiceName)) {}

// Destructor
CEventLog::~CEventLog() {
    if (m_hEventSource) {
        ::DeregisterEventSource(m_hEventSource);
    }
}

void CEventLog::LogEvent(WORD wType, DWORD dwID, 
                         std::optional<std::string> pszS1, 
                         std::optional<std::string> pszS2, 
                         std::optional<std::string> pszS3) {
    if (!m_hEventSource) {
        m_hEventSource = ::RegisterEventSourceA(nullptr, serviceName.c_str());
        if (!m_hEventSource) {
            return;
        }
    }

    // Use a vector instead of raw pointers
    std::vector<std::string> messages;
    if (pszS1) messages.push_back(*pszS1);
    if (pszS2) messages.push_back(*pszS2);
    if (pszS3) messages.push_back(*pszS3);

    // Convert strings to C-style for ReportEventA
    std::vector<const char*> pStrings;
    for (const auto& msg : messages) {
        pStrings.push_back(msg.c_str());
    }

    // Use `std::span` to pass dynamic array safely
    std::span<const char*> stringSpan = pStrings;

    ::ReportEventA(m_hEventSource, wType, 0, dwID, nullptr, 
                   static_cast<WORD>(stringSpan.size()), 0, 
                   stringSpan.data(), nullptr);
}
