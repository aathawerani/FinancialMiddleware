#include "DateTime.h"
#include <iostream>
#include <format>
#include <iomanip>
#include <sstream>
#include <chrono>

CDateTime::CDateTime() {
    GetLocalTime(&systemTime);
}

CDateTime::CDateTime(const SYSTEMTIME& timeStruct) 
    : systemTime(timeStruct) {}

CDateTime::CDateTime(std::string_view cDateTime) {
    if (cDateTime.length() != 14) {
        throw std::invalid_argument("Invalid datetime format. Expected YYYYMMDDHHMMSS.");
    }

    systemTime.wYear   = std::stoi(std::string(cDateTime.substr(0, 4)));
    systemTime.wMonth  = std::stoi(std::string(cDateTime.substr(4, 2)));
    systemTime.wDay    = std::stoi(std::string(cDateTime.substr(6, 2)));
    systemTime.wHour   = std::stoi(std::string(cDateTime.substr(8, 2)));
    systemTime.wMinute = std::stoi(std::string(cDateTime.substr(10, 2)));
    systemTime.wSecond = std::stoi(std::string(cDateTime.substr(12, 2)));
    systemTime.wMilliseconds = 0;
}

SYSTEMTIME CDateTime::getSystemTime() const {
    return systemTime;
}

UINT64 CDateTime::getTicks() const {
    FILETIME fileTime;
    SystemTimeToFileTime(&systemTime, &fileTime);
    
    ULARGE_INTEGER uLargeInteger{};
    uLargeInteger.LowPart  = fileTime.dwLowDateTime;
    uLargeInteger.HighPart = fileTime.dwHighDateTime;

    return uLargeInteger.QuadPart;
}

bool CDateTime::refresh() {
    GetLocalTime(&systemTime);
    return true;
}

bool CDateTime::addSeconds(unsigned int seconds) {
    FILETIME fileTime;
    if (!SystemTimeToFileTime(&systemTime, &fileTime)) {
        return false;
    }

    ULARGE_INTEGER uLargeInteger{};
    uLargeInteger.LowPart  = fileTime.dwLowDateTime;
    uLargeInteger.HighPart = fileTime.dwHighDateTime;

    uLargeInteger.QuadPart += static_cast<UINT64>(seconds) * SECOND;

    fileTime.dwLowDateTime  = uLargeInteger.LowPart;
    fileTime.dwHighDateTime = uLargeInteger.HighPart;

    return FileTimeToSystemTime(&fileTime, &systemTime);
}

bool CDateTime::addDays(int days) {
    return addSeconds(days * (DAY / SECOND));
}

bool CDateTime::getWeekDay(std::string &buffer) {
    wchar_t wBuffer[DT_STRING_LEN]; // Wide char buffer

    if (GetDateFormatW(LOCALE_USER_DEFAULT, 0, &systemTime, L"dddd", wBuffer, DT_STRING_LEN) > 0) {
        // Convert wide string (wchar_t) to narrow string (char)
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        buffer = converter.to_bytes(wBuffer);
        return true;
    }
    
    return false;
}

std::string CDateTime::toString() const {
    return std::format("{:04}{:02}{:02}{:02}{:02}{:02}",
                        systemTime.wYear, systemTime.wMonth, systemTime.wDay,
                        systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
}

std::string CDateTime::toStringFull() const {
    return std::format("{:04}{:02}{:02}{:02}{:02}{:02}{:03}",
                        systemTime.wYear, systemTime.wMonth, systemTime.wDay,
                        systemTime.wHour, systemTime.wMinute, systemTime.wSecond,
                        systemTime.wMilliseconds);
}

std::string CDateTime::toGMTString() const {
    SYSTEMTIME universalTime;
    GetSystemTime(&universalTime);
    return std::format("{:04}{:02}{:02}{:02}{:02}{:02}",
                        universalTime.wYear, universalTime.wMonth, universalTime.wDay,
                        universalTime.wHour, universalTime.wMinute, universalTime.wSecond);
}

std::string CDateTime::toDateString() const {
    return std::format("{:04}{:02}{:02}",
                        systemTime.wYear, systemTime.wMonth, systemTime.wDay);
}

std::string CDateTime::toTimeString() const {
    return std::format("{:02}{:02}{:02}",
                        systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
}

std::string CDateTime::toFormattedString() const {
    return std::format("{:04}/{:02}/{:02} {:02}:{:02}:{:02}",
                        systemTime.wYear, systemTime.wMonth, systemTime.wDay,
                        systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
}
