#include <iostream>
#include "EventLog.h"

int main() {
    CEventLog eventLog("MyService");

    // Using C++20 std::optional without explicit std::optional<std::string> (thanks to implicit conversion)
    eventLog.LogEvent(EVENTLOG_INFORMATION_TYPE, 1001, "Initialization successful.");
    eventLog.LogEvent(EVENTLOG_WARNING_TYPE, 2001, "Warning: Low disk space.", "Drive C:");
    eventLog.LogEvent(EVENTLOG_ERROR_TYPE, 0, "Error: Service failed to start.", "Error Code: 500", "Restart recommended.");

    std::cout << "Events logged successfully!..." << std::endl;
    return 0;
}
