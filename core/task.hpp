#pragma once
#include <string>
#include <chrono>

class Task {
public:
    
    Task(int id,
         const std::string& title,
         const std::string& description,
         bool completed = false);

    Task(int id,
         const std::string& title,
         const std::string& description,
         std::chrono::system_clock::time_point deadline,
         bool completed = false);

    
    int getId() const;

    const std::string& getTitle() const;

    void setTitle(const std::string& title);
    
    const std::string& getDescription() const;
    
    void setDescription(const std::string& description);
    
    bool isCompleted() const;
    
    void setCompleted(bool completed);
    
    std::chrono::system_clock::time_point getDeadline() const;
    void setDeadline(std::chrono::system_clock::time_point deadline);

    
    bool isOverdue() const;

private:
    int id_;
    std::string title_;
    std::string description_;
    bool completed_;
    std::chrono::system_clock::time_point deadline_;
};
