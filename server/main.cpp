#include <iostream>
#include "../core/task.hpp"
#include "../core/task_manager.cpp"

int main() {
    addTask("First task");
    std::cout << "Server started\n";
    return 0;
}
