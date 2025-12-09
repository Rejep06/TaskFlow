#include "task_manager.hpp"

TaskManager::TaskManager()
    : nextTaskId_(1) {
}

Task& TaskManager::createTask(const std::string& title,
                              const std::string& description) {
    int id = nextTaskId_;
    nextTaskId_ += 1;
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