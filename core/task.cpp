#include "task.hpp"

Task::Task(int id,
           const std::string& title,
           const std::string& description,
           bool completed = false)
    : id_(id), title_(title), description_(description), completed_(completed) {
}

int Task::getId() const {
    return id_;
}

const std::string& Task::getTitle() const {
    return title_;
}

void Task::setTitle(const std::string& title) {
    title_ = title;
}

const std::string& Task::getDescription() const {
    return description_;
}

void Task::setDescription(const std::string& description) {
    description_ = description;
}

bool Task::isCompleted() const {
    return completed_;
}

void Task::setCompleted(bool completed) {
    completed_ = completed;
}