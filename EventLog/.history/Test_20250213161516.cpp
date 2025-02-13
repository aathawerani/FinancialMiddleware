#include <iostream>
#include "EventLog.h"

int main() {
    CEventLog eventLog("MyService");

    // Corrected Calls - Using std::optional<std::string>
    eventLog.LogEvent(EVENTLOG_INFORMATION_TYPE, 1001, std::optional<std::string>("Initialization successful."));
    eventLog.LogEvent(EVENTLOG_WARNING_TYPE, 2001, std::optional<std::string>("Warning: Low disk space."), std::optional<std::string>("Drive C:"));
    eventLog.LogEvent(EVENTLOG_ERROR_TYPE, 3001, std::optional<std::string>("Error: Service failed to start."),
                       std::optional<std::string>("Error Code: 500"), std::optional<std::string>("Restart recommended."));

    std::cout << "Events logged successfully!...." << std::endl;
    return 0;
}
