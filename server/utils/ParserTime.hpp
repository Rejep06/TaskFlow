#pragma once
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

class ParserTime {
   public:
    static std::string timePointToString(const std::chrono::system_clock::time_point& tp);

    static std::chrono::system_clock::time_point stringToTimePoint(const std::string& s);
};
