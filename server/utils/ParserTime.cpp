#include "ParserTime.hpp"

#include <cstdio>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string ParserTime::timePointToString(const std::chrono::system_clock::time_point& tp) {
    if (tp == std::chrono::system_clock::time_point::max()) {
        return "";
    }

    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};

#ifdef _WIN32
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif

    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &tm);
    return buf;
}

std::chrono::system_clock::time_point ParserTime::stringToTimePoint(const std::string& s) {
    if (s.empty()) {
        return std::chrono::system_clock::time_point::max();
    }

    int Y, M, D, h, m;

    // Strict parsing: "YYYY-MM-DD HH:MM"
    if (std::sscanf(s.c_str(), "%d-%d-%d %d:%d", &Y, &M, &D, &h, &m) != 5) {
        throw std::invalid_argument("Invalid datetime format: " + s);
    }

    std::tm tm{};
    tm.tm_year = Y - 1900;
    tm.tm_mon = M - 1;
    tm.tm_mday = D;
    tm.tm_hour = h;
    tm.tm_min = m;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;  // no daylight savings correction

#ifdef _WIN32
    std::time_t t = _mkgmtime(&tm);
#else
    std::time_t t = timegm(&tm);
#endif

    if (t == -1) {
        throw std::invalid_argument("Invalid datetime (range or values): " + s);
    }

    return std::chrono::system_clock::from_time_t(t);
}
