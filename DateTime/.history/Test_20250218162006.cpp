#include "DateTime.h"
#include <iostream>
#include <string>

int main() {
    // Create CDateTime object with current local time
    CDateTime dateTime;
    SYSTEMTIME systemTime = dateTime.getSystemTime();
    std::cout << "Current local time: " << systemTime.wYear << "-" << systemTime.wMonth << "-" << systemTime.wDay << " "
              << systemTime.wHour << ":" << systemTime.wMinute << ":" << systemTime.wSecond << std::endl;

    // Create CDateTime object with specific SYSTEMTIME
    SYSTEMTIME specificTime = {2023, 10, 0, 15, 10, 30, 45, 0};
    CDateTime dateTimeSpecific(specificTime);
    std::cout << "Specific time: " << dateTimeSpecific.getSystemTime().wYear << "-" << dateTimeSpecific.getSystemTime().wMonth << "-"
              << dateTimeSpecific.getSystemTime().wDay << " " << dateTimeSpecific.getSystemTime().wHour << ":"
              << dateTimeSpecific.getSystemTime().wMinute << ":" << dateTimeSpecific.getSystemTime().wSecond << std::endl;

    // Create CDateTime object with string
    std::string dateTimeStr = "20231015103045";
    CDateTime dateTimeFromString(dateTimeStr);
    std::cout << "DateTime from string: " << dateTimeFromString.getSystemTime().wYear << "-" << dateTimeFromString.getSystemTime().wMonth << "-"
              << dateTimeFromString.getSystemTime().wDay << " " << dateTimeFromString.getSystemTime().wHour << ":"
              << dateTimeFromString.getSystemTime().wMinute << ":" << dateTimeFromString.getSystemTime().wSecond << std::endl;

    // Get ticks
    ULONG64 ticks = dateTime.getTicks();
    std::cout << "Ticks: " << ticks << std::endl;

    // Refresh time
    dateTime.refresh();
    std::cout << "Refreshed time: " << dateTime.getSystemTime().wYear << "-" << dateTime.getSystemTime().wMonth << "-"
              << dateTime.getSystemTime().wDay << " " << dateTime.getSystemTime().wHour << ":"
              << dateTime.getSystemTime().wMinute << ":" << dateTime.getSystemTime().wSecond << std::endl;

    // Add seconds
    dateTime.addSeconds(3600); // Add 1 hour
    std::cout << "Time after adding 3600 seconds: " << dateTime.getSystemTime().wYear << "-" << dateTime.getSystemTime().wMonth << "-"
              << dateTime.getSystemTime().wDay << " " << dateTime.getSystemTime().wHour << ":"
              << dateTime.getSystemTime().wMinute << ":" << dateTime.getSystemTime().wSecond << std::endl;

    // Add days
    dateTime.addDays(1); // Add 1 day
    std::cout << "Time after adding 1 day: " << dateTime.getSystemTime().wYear << "-" << dateTime.getSystemTime().wMonth << "-"
              << dateTime.getSystemTime().wDay << " " << dateTime.getSystemTime().wHour << ":"
              << dateTime.getSystemTime().wMinute << ":" << dateTime.getSystemTime().wSecond << std::endl;

    // Get week day
    char weekDay[DT_STRING_LEN];
    if (dateTime.getWeekDay(weekDay)) {
        std::cout << "Week day: " << weekDay << std::endl;
    } else {
        std::cout << "Failed to get week day" << std::endl;
    }

    // Convert to string
    std::string dateTimeString;
    dateTime.toString(dateTimeString);
    std::cout << "DateTime string: " << dateTimeString << std::endl;

    // Convert to full string
    std::string dateTimeFullString;
    dateTime.toStringFull(dateTimeFullString);
    std::cout << "DateTime full string: " << dateTimeFullString << std::endl;

    // Convert to formatted string
    std::string formattedString;
    dateTime.toFormattedString(formattedString);
    std::cout << "Formatted DateTime string: " << formattedString << std::endl;

    // Convert to GMT string
    std::string gmtString;
    dateTime.toGMTString(gmtString);
    std::cout << "GMT DateTime string: " << gmtString << std::endl;

    // Convert to date string
    std::string dateString;
    dateTime.toDateString(dateString);
    std::cout << "Date string: " << dateString << std::endl;

    // Convert to time string
    std::string timeString;
    dateTime.toTimeString(timeString);
    std::cout << "Time string: " << timeString << std::endl;

    return 0;
}
