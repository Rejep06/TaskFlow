#include "task.hpp"
#include <chrono>

Task::Task(int id,
           const std::string& title,
           const std::string& description,
           bool completed)
    : id_(id)
    , title_(title)
    , description_(description)
    , completed_(completed)
    , deadline_(std::chrono::system_clock::time_point::max()) {
}

Task::Task(int id,
           const std::string& title,
           const std::string& description,
           std::chrono::system_clock::time_point deadline,
           bool completed)
    : id_(id)
    , title_(title)
    , description_(description)
    , completed_(completed)
    , deadline_(deadline) {
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

std::chrono::system_clock::time_point Task::getDeadline() const {
    return deadline_;
}

void Task::setDeadline(std::chrono::system_clock::time_point deadline) {
    deadline_ = deadline;
}

bool Task::isOverdue() const {
    if (completed_) return false;
    auto now = std::chrono::system_clock::now();
    return deadline_ != std::chrono::system_clock::time_point::max() && now > deadline_;
}