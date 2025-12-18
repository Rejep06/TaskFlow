#include "task.hpp"

// Constructors
Task::Task(int id,
           const std::string& title,
           const std::string& description,
           bool completed)
    : id_(id), title_(title), description_(description), completed_(completed), deadline_(std::chrono::system_clock::time_point::max()) {
    notified_day_ = false;
    notified_hour_ = false;
    notified_10min_ = false;
    notified_deadline_ = false;
}

Task::Task(int id,
           const std::string& title,
           const std::string& description,
           std::chrono::system_clock::time_point deadline,
           bool completed)
    : id_(id), title_(title), description_(description), completed_(completed), deadline_(deadline) {
    notified_day_ = false;
    notified_hour_ = false;
    notified_10min_ = false;
    notified_deadline_ = false;
}

// Getters
int Task::getId() const {
    return id_;
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

bool Task::getNotified_day() const {
    return notified_day_;
}

bool Task::getNotified_hour() const {
    return notified_hour_;
}

bool Task::getNotified_10min() const {
    return notified_10min_;
}

bool Task::getNotified_deadline() const {
    return notified_deadline_;
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

// void Task::setNotified_day(bool q) {
//     notified_day_ = q;
// }

// void Task::setNotified_hour(bool q) {
//     notified_hour_ = q;
// }

// void Task::setNotified_10min(bool q) {
//     notified_10min_ = q;
// }

// void Task::setNotified_deadline(bool q) {
//     notified_deadline_ = q;
// }

// Functions
bool Task::isOverdue() const {
    if (completed_) return false;
    auto now = std::chrono::system_clock::now();
    return deadline_ != std::chrono::system_clock::time_point::max() && now > deadline_;
}

std::optional<NotifyType> Task::checkNotification() {
    if (completed_) return std::nullopt;

    auto now = std::chrono::system_clock::now();
    auto diff = deadline_ - now;

    if (diff <= std::chrono::minutes(0) && !notified_deadline_) {
        notified_deadline_ = true;
        return NotifyType::DEADLINE;
    }

    if (diff <= std::chrono::minutes(10) && !notified_10min_) {
        notified_10min_ = true;
        return NotifyType::MIN10;
    }

    if (diff <= std::chrono::hours(1) && !notified_hour_) {
        notified_hour_ = true;
        return NotifyType::HOUR;
    }

    if (diff <= std::chrono::hours(24) && !notified_day_) {
        notified_day_ = true;
        return NotifyType::DAY;
    }

    return std::nullopt;
}
