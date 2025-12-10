#include "task_manager.hpp"

#include <chrono>

// Constructors
TaskManager::TaskManager()
    : nextTaskId_(1) {
}

// ITaskService interface implementation
Task& TaskManager::createTask(const std::string& title,
                              const std::string& description,
                              std::optional<std::chrono::system_clock::time_point> deadline) {
    int id = nextTaskId_++;
    if (deadline.has_value()) {
        tasks_.emplace_back(id, title, description, deadline.value(), false);
    } else {
        tasks_.emplace_back(id, title, description, false);
    }
    return tasks_.back();
}

void TaskManager::addTask(const Task& task) {
    tasks_.push_back(task);
    if (task.getId() >= nextTaskId_) {
        nextTaskId_ = task.getId() + 1;
    }
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

std::vector<Task*> TaskManager::getOverdueTasks() {
    std::vector<Task*> overdue;
    for (auto& task : tasks_) {
        if (task.isOverdue()) {
            overdue.push_back(&task);
        }
    }
    return overdue;
}