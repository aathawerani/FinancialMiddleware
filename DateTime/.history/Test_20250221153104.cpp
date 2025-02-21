#include "DateTime.h"
#include <iostream>
#include <string>
#include <format>

int main() {
    try {
        // Create CDateTime object with current local time
        CDateTime dateTime;
        SYSTEMTIME systemTime = dateTime.getSystemTime();
        std::cout << std::format("Current local time: {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
            systemTime.wYear, systemTime.wMonth, systemTime.wDay,
            systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

        // Create CDateTime object with specific SYSTEMTIME
        SYSTEMTIME specificTime = {2023, 10, 0, 15, 10, 30, 45, 0};
        CDateTime dateTimeSpecific(specificTime);
        SYSTEMTIME st = dateTimeSpecific.getSystemTime();
        std::cout << std::format("Specific time: {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

        // Create CDateTime object with string
        std::string dateTimeStr = "20231015103045";
        CDateTime dateTimeFromString(dateTimeStr);
        SYSTEMTIME stStr = dateTimeFromString.getSystemTime();
        std::cout << std::format("DateTime from string: {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
            stStr.wYear, stStr.wMonth, stStr.wDay, stStr.wHour, stStr.wMinute, stStr.wSecond);

        // Get ticks
        UINT64 ticks = dateTime.getTicks();
        std::cout << "Ticks: " << ticks << "\n";

        // Refresh time
        dateTime.refresh();
        SYSTEMTIME refreshedTime = dateTime.getSystemTime();
        std::cout << std::format("Refreshed time: {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
            refreshedTime.wYear, refreshedTime.wMonth, refreshedTime.wDay,
            refreshedTime.wHour, refreshedTime.wMinute, refreshedTime.wSecond);

        // Add seconds
        dateTime.addSeconds(3600); // Add 1 hour
        SYSTEMTIME addedTime = dateTime.getSystemTime();
        std::cout << std::format("Time after adding 3600 seconds: {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
            addedTime.wYear, addedTime.wMonth, addedTime.wDay,
            addedTime.wHour, addedTime.wMinute, addedTime.wSecond);

        // Add days
        dateTime.addDays(1); // Add 1 day
        SYSTEMTIME addedDay = dateTime.getSystemTime();
        std::cout << std::format("Time after adding 1 day: {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
            addedDay.wYear, addedDay.wMonth, addedDay.wDay,
            addedDay.wHour, addedDay.wMinute, addedDay.wSecond);

        // ✅ Fix: Get week day properly
        std::string weekDay;
        if (dateTime.getWeekDay(weekDay)) {
            std::cout << "Week day: " << weekDay << "\n";
        } else {
            std::cerr << "Failed to get the day of the week.\n";
        }

        // Convert to string formats
        std::cout << "DateTime string: " << dateTime.toString() << "\n";
        std::cout << "DateTime full string: " << dateTime.toStringFull() << "\n";
        std::cout << "Formatted DateTime string: " << dateTime.toFormattedString() << "\n";
        std::cout << "GMT DateTime string: " << dateTime.toGMTString() << "\n";
        std::cout << "Date string: " << dateTime.toDateString() << "\n";
        std::cout << "Time string: " << dateTime.toTimeString() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
