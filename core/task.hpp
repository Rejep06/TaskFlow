#pragma once
#include <string>
#include <chrono>

class Task {
private:
    int id_;
    std::string title_;
    std::string description_;
    bool completed_;
    std::chrono::system_clock::time_point deadline_;

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

    // Setters
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setCompleted(bool completed);
    void setDeadline(std::chrono::system_clock::time_point deadline);

    // Functions
    bool isOverdue() const;
};