#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "ITaskService.hpp"
#include "task.hpp"

class TaskManager : public ITaskService {
   public:
    TaskManager();
    Task& createTask(const std::string& title,
                     const std::string& description) override;

    const std::vector<Task>& getAllTasks() const override;

    Task* findTaskById(int id) override;

    bool deleteTask(int id) override;

   private:
    int nextTaskId_;
    std::vector<Task> tasks_;
};
