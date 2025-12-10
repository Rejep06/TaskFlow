#pragma once
#include <optional>
#include <string>
#include <vector>

#include "task.hpp"

class ITaskService {
   public:
    virtual ~ITaskService() = default;

    virtual const std::vector<Task>& getAllTasks() const = 0;
    virtual Task& createTask(const std::string&, const std::string&,
                             std::optional<std::chrono::system_clock::time_point> deadline) = 0;
    virtual Task* findTaskById(int) = 0;
    virtual bool deleteTask(int) = 0;
    virtual void addTask(const Task&) {
        // Временная реализация, можно переопределить в наследниках
        return;
    }
};
