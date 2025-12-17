#pragma once

#include <algorithm>
#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "ITaskService.hpp"
#include "task.hpp"

class TaskManager : public ITaskService {
private:
    int nextTaskId_;
    std::vector<Task> tasks_;

public:
    TaskManager();

    Task& createTask(const std::string& username,
                     const std::string& title,
                     const std::string& description,
                     std::optional<std::chrono::system_clock::time_point> deadline) override;

    const std::vector<Task>& getAllTasks(const std::string& username) const override;

    Task* findTaskById(const std::string& username, int id) override;

    bool deleteTask(const std::string& username, int id) override;

    std::vector<Task*> getOverdueTasks(const std::string& username) override;

    void addTask(const Task& task) override;
};