#pragma once
#include <string>
#include <thread>

class ConsoleApp {
   public:
    explicit ConsoleApp(const std::string& baseUrl);

    void run();

   private:
    std::string baseUrl_;

    void notificationLoop();

    void printMenu();
    void clearInput();

    void addTask();
    void listTasks();
    void editTask();
    void toggleTaskStatus();
    void deleteTask();
    void showOverdueTasks();
};