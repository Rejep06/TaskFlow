#pragma once

#include <algorithm>
#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "ITaskService.hpp"
#include "task.hpp"

class TaskManager : public ITaskService {
   public:
    TaskManager();

    Task& createTask(const std::string& title,
                     const std::string& description,
                     std::optional<std::chrono::system_clock::time_point> deadline) override;

    const std::vector<Task>& getAllTasks() const override;

    Task* findTaskById(int id) override;

    bool deleteTask(int id) override;

    std::vector<Task*> getOverdueTasks() override;

    void addTask(const Task& task) override;

    std::vector<Task>& getAllTasks() override;

   private:
    int nextTaskId_;
    std::vector<Task> tasks_;
};
