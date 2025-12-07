#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include "task.hpp"

class TaskManager {
public:
    TaskManager()
        : nextTaskId_(1) {
    }

    Task& createTask(const std::string& title,
                     const std::string& description) {
        int id = nextTaskId_;
        nextTaskId_ += 1;
        tasks_.emplace_back(id, title, description, false);
        return tasks_.back();
    }

    const std::vector<Task>& getAllTasks() const {
        return tasks_;
    }

    Task* findTaskById(int id) {
        for (auto& task : tasks_) {
            if (task.getId() == id) {
                return &task;
            }
        }
        return nullptr;
    }

    bool deleteTask(int id) {
        auto it = std::remove_if(
            tasks_.begin(),
            tasks_.end(),
            [id](const Task& task) {
                return task.getId() == id;
            }
        );
        if (it == tasks_.end()) {
            return false;
        }
        tasks_.erase(it, tasks_.end());
        return true;
    }

private:
    int nextTaskId_;
    std::vector<Task> tasks_;
};

