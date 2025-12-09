#pragma once
#include <vector>
#include "task.hpp"

class TaskRepository {
public:
    std::vector<Task> load();
    void save(const std::vector<Task>& tasks);

private:
    const std::string filename = "data/tasks.json";
};
