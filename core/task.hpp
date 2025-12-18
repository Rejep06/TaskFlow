#pragma once
#include <chrono>
#include <optional>
#include <string>

enum class NotifyType {
    DAY,
    HOUR,
    MIN10,
    DEADLINE
};

class Task {
   private:
    int id_;
    std::string title_;
    std::string description_;
    bool completed_;
    std::chrono::system_clock::time_point deadline_;
    bool notified_day_;
    bool notified_hour_;
    bool notified_10min_;
    bool notified_deadline_;

   public:
    // Constructors
    Task(int id,
         const std::string& title,
         const std::string& description,
         bool completed = false);

    Task(int id,
         const std::string& title,
         const std::string& description,
         std::chrono::system_clock::time_point deadline,
         bool completed = false);

    // Getters
    int getId() const;
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    bool isCompleted() const;
    std::chrono::system_clock::time_point getDeadline() const;
//     bool getNotified_day() const;
//     bool getNotified_hour() const;
//     bool getNotified_10min() const;
//     bool getNotified_deadline() const;

    // Setters
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setCompleted(bool completed);
    void setDeadline(std::chrono::system_clock::time_point deadline);
//     void setNotified_day(bool q);
//     void setNotified_hour(bool q);
//     void setNotified_10min(bool q);
//     void setNotified_deadline(bool q);

    // Functions
    bool isOverdue() const;
    std::optional<NotifyType> checkNotification();
};