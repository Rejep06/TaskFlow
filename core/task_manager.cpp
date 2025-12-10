#include "task_manager.hpp"

// Constructors
TaskManager::TaskManager()
    : nextTaskId_(1) {
}

// ITaskService interface implementation
Task& TaskManager::createTask(const std::string& title,
                              const std::string& description) {
    int id = nextTaskId_++;
    tasks_.emplace_back(id, title, description, false);
    return tasks_.back();
}

const std::vector<Task>& TaskManager::getAllTasks() const {
    return tasks_;
}

Task* TaskManager::findTaskById(int id) {
    for (auto& task : tasks_) {
        if (task.getId() == id) {
            return &task;
        }
    }
    return nullptr;
}

bool TaskManager::deleteTask(int id) {
    auto it = std::remove_if(
        tasks_.begin(),
        tasks_.end(),
        [id](const Task& task) {
            return task.getId() == id;
        });
    if (it == tasks_.end()) {
        return false;
    }
    tasks_.erase(it, tasks_.end());
    return true;
}

// Functions
Task& TaskManager::createTaskWithDeadline(const std::string& title,
                                          const std::string& description,
                                          std::chrono::system_clock::time_point deadline) {
    int id = nextTaskId_++;
    tasks_.emplace_back(id, title, description, deadline, false);
    return tasks_.back();
}

std::vector<Task*> TaskManager::getOverdueTasks() {
    std::vector<Task*> overdue;
    for (auto& task : tasks_) {
        if (task.isOverdue()) {
            overdue.push_back(&task);
        }
    }
    return overdue;
}