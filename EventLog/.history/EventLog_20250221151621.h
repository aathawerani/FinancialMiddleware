#include <windows.h>
#include <string>
#include <vector>
#include <optional>

constexpr size_t MAX_STRING_LENGTH = 256;

class CEventLog {
public:
    explicit CEventLog(std::string_view szServiceName);
    void SetServiceName(std::string_view szServiceName);
    virtual ~CEventLog();

    void LogEvent(WORD wType, DWORD dwID,
                  std::optional<std::string_view> pszS1 = std::nullopt,
                  std::optional<std::string_view> pszS2 = std::nullopt,
                  std::optional<std::string_view> pszS3 = std::nullopt);

private:
    HANDLE m_hEventSource = nullptr;
    std::string serviceName;
};
