#include <windows.h>
#include <string>
#include <array>
#include <vector>
#include <optional>
#include <cstring>  // for std::strcpy

constexpr size_t MAX_STRING_LENGTH = 256;

class CEventLog {
public:
    explicit CEventLog(std::string szServiceName);
    void SetServiceName(std::string szServiceName);
    virtual ~CEventLog();

    void LogEvent(WORD wType, DWORD dwID,
                  std::optional<std::string> pszS1 = std::nullopt,
                  std::optional<std::string> pszS2 = std::nullopt,
                  std::optional<std::string> pszS3 = std::nullopt);

private:
    HANDLE m_hEventSource = nullptr;
    std::string serviceName;
};

