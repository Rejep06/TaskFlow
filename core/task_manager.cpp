#include "task_manager.hpp"

#include <chrono>

// Constructors
TaskManager::TaskManager()
    : nextTaskId_(1) {
}

// ITaskService interface implementation
Task& TaskManager::createTask(const std::string& username,
                              const std::string& title,
                              const std::string& description,
                              std::optional<std::chrono::system_clock::time_point> deadline) {
    int id = nextTaskId_++;
    if (deadline.has_value()) {
        tasks_.emplace_back(id, username, title, description, deadline.value(), false);
    } else {
        tasks_.emplace_back(id, username, title, description, false);
    }
    return tasks_.back();
}

void TaskManager::addTask(const Task& task) {
    tasks_.push_back(task);
    if (task.getId() >= nextTaskId_) {
        nextTaskId_ = task.getId() + 1;
    }
}

const std::vector<Task>& TaskManager::getAllTasks(const std::string& username) const {
    static std::vector<Task> filteredTasks;
    filteredTasks.clear();
    
    for (const auto& task : tasks_) {
        if (task.getUsername() == username) {
            filteredTasks.push_back(task);
        }
    }
    
    return filteredTasks;
}

Task* TaskManager::findTaskById(const std::string& username, int id) {
    for (auto& task : tasks_) {
        if (task.getId() == id && task.getUsername() == username) {
            return &task;
        }
    }
    return nullptr;
}

bool TaskManager::deleteTask(const std::string& username, int id) {
    auto it = std::remove_if(
        tasks_.begin(),
        tasks_.end(),
        [username, id](const Task& task) {
            return task.getId() == id && task.getUsername() == username;
        });
    if (it == tasks_.end()) {
        return false;
    }
    tasks_.erase(it, tasks_.end());
    return true;
}

std::vector<Task*> TaskManager::getOverdueTasks(const std::string& username) {
    std::vector<Task*> overdue;
    for (auto& task : tasks_) {
        if (task.getUsername() == username && task.isOverdue()) {
            overdue.push_back(&task);
        }
    }
    return overdue;
}