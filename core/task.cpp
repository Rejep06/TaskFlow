#include "task.hpp"

// Constructors
Task::Task(int id,
           const std::string& username,
           const std::string& title,
           const std::string& description,
           bool completed)
    : id_(id)
    , username_(username)
    , title_(title)
    , description_(description)
    , completed_(completed)
    , deadline_(std::chrono::system_clock::time_point::max()) {
}

Task::Task(int id,
           const std::string& username,
           const std::string& title,
           const std::string& description,
           std::chrono::system_clock::time_point deadline,
           bool completed)
    : id_(id)
    , username_(username)
    , title_(title)
    , description_(description)
    , completed_(completed)
    , deadline_(deadline) {
}

// Getters
int Task::getId() const {
    return id_;
}

const std::string& Task::getUsername() const {
    return username_;
}

const std::string& Task::getTitle() const {
    return title_;
}

const std::string& Task::getDescription() const {
    return description_;
}

bool Task::isCompleted() const {
    return completed_;
}

std::chrono::system_clock::time_point Task::getDeadline() const {
    return deadline_;
}

// Setters
void Task::setTitle(const std::string& title) {
    title_ = title;
}

void Task::setDescription(const std::string& description) {
    description_ = description;
}

void Task::setCompleted(bool completed) {
    completed_ = completed;
}

void Task::setDeadline(std::chrono::system_clock::time_point deadline) {
    deadline_ = deadline;
}

// Functions
bool Task::isOverdue() const {
    if (completed_) return false;
    auto now = std::chrono::system_clock::now();
    return deadline_ != std::chrono::system_clock::time_point::max() && now > deadline_;
}