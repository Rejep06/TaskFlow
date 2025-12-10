#pragma once
#include <string>

class ConsoleApp {
   public:
    explicit ConsoleApp(const std::string& baseUrl);

    void run();

   private:
    std::string baseUrl_;

    void printMenu();
    void clearInput();

    void addTask();
    void listTasks();
    void editTask();
    void toggleTaskStatus();
    void deleteTask();
    void showOverdueTasks();
};