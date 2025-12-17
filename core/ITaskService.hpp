#pragma once
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "task.hpp"

class ITaskService {
   public:
    virtual ~ITaskService() = default;

    virtual const std::vector<Task>& getAllTasks(const std::string& username) const = 0;
    virtual Task& createTask(const std::string& username,
                             const std::string& title,
                             const std::string& description,
                             std::optional<std::chrono::system_clock::time_point> deadline) = 0;
    virtual Task* findTaskById(const std::string& username, int id) = 0;
    virtual bool deleteTask(const std::string& username, int id) = 0;
    virtual void addTask(const Task& task) = 0;

    virtual std::vector<Task*> getOverdueTasks(const std::string& username) = 0;
};