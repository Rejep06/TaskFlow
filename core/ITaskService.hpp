#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include "task.hpp"

class ITaskService {
public:
    virtual ~ITaskService() = default;

    virtual const std::vector<Task>& getAllTasks() const = 0;
    virtual Task& createTask(const std::string& title, 
                             const std::string& description) = 0;
    virtual Task* findTaskById(int id) = 0;
    virtual bool deleteTask(int id) = 0;
    
    // Временная реализация (можно переопределить в наследниках)
    virtual void addTask(const Task& task) {
        // Базовая реализация выбрасывает исключение, 
        // чтобы показать что метод не реализован
        throw std::runtime_error("addTask() not implemented");
    }
};