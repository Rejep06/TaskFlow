#include "ParserTime.hpp"

std::string ParserTime::timePointToString(const std::chrono::system_clock::time_point& tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
#ifdef _WIN32
    gmtime_s(&tm, &t);
#else
    tm = *std::gmtime(&t);
#endif
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);
    return buf;
}

std::chrono::system_clock::time_point ParserTime::stringToTimePoint(const std::string& s) {
    std::tm tm{};
#ifdef _WIN32
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    tm.tm_isdst = -1;
    std::time_t t = _mkgmtime(&tm);
#else
    strptime(s.c_str(), "%Y-%m-%dT%H:%M:%SZ", &tm);
    std::time_t t = timegm(&tm);
#endif
    return std::chrono::system_clock::from_time_t(t);
}