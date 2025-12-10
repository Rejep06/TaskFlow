#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

#include "ITaskService.hpp"
#include "task.hpp"

class TaskManager : public ITaskService {
private:
    int nextTaskId_;
    std::vector<Task> tasks_;

public:
    // Constructors
    TaskManager();

    // ITaskService interface implementation
    Task& createTask(const std::string& title,
                     const std::string& description) override;
    
    const std::vector<Task>& getAllTasks() const override;
    
    Task* findTaskById(int id) override;
    
    bool deleteTask(int id) override;

    // Functions
    Task& createTaskWithDeadline(const std::string& title,
                                 const std::string& description,
                                 std::chrono::system_clock::time_point deadline);

    std::vector<Task*> getOverdueTasks();
};