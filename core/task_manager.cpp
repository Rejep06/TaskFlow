#include <vector>

#include "task.hpp"

std::vector<Task> tasks;

int addTask(const std::string& title) {
    int id = tasks.size() + 1;
    tasks.emplace_back(id, title, false);
    return id;
}