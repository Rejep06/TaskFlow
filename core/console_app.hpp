#pragma once

#include <iostream>
#include <string>
#include <limits>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "task_manager.hpp"

class ConsoleApp {
private:
    TaskManager& manager_;

    // Helper functions
    std::chrono::system_clock::time_point parseDateTime(const std::string& s);
    std::string formatDateTime(std::chrono::system_clock::time_point tp);

    // Input/output utilities
    void clearInput();
    void printMenu();

    // Menu handlers
    void addTask();
    void listTasks();
    void editTask();
    void toggleTaskStatus();
    void deleteTask();
    void showOverdueTasks();

public:
    explicit ConsoleApp(TaskManager& manager);

    void run();
};