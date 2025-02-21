#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <windows.h>
#include <string>
#include <chrono>
#include <string_view>

// Time related constants using constexpr
constexpr INT64 SECOND = 10'000'000;
constexpr INT64 MINUTE = 60 * SECOND;
constexpr INT64 HOUR   = 60 * MINUTE;
constexpr INT64 DAY    = 24 * HOUR;

class CDateTime  
{
private:
    SYSTEMTIME systemTime{};

public:
    CDateTime();
    explicit CDateTime(const SYSTEMTIME& timeStruct);
    explicit CDateTime(std::string_view dateTimeString);

    [[nodiscard]] UINT64 getTicks() const;
    bool refresh();
    bool addSeconds(unsigned int seconds);
    bool addDays(int days);

    [[nodiscard]] bool getWeekDay(std::string& buffer); 
    [[nodiscard]] std::string toString() const;
    [[nodiscard]] std::string toStringFull() const;
    [[nodiscard]] std::string toGMTString() const;
    [[nodiscard]] std::string toDateString() const;
    [[nodiscard]] std::string toTimeString() const;
    [[nodiscard]] std::string toFormattedString() const;

    [[nodiscard]] SYSTEMTIME getSystemTime() const;

    virtual ~CDateTime() = default;
};

#endif // DATE_TIME_H
