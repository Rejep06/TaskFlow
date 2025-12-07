#pragma once
#include <string>

class Task {
public:
    Task(int id,
         const std::string& title,
         const std::string& description,
         bool completed = false)
        : id_(id)
        , title_(title)
        , description_(description)
        , completed_(completed) {
    }

    int getId() const {
        return id_;
    }

    const std::string& getTitle() const {
        return title_;
    }

    void setTitle(const std::string& title) {
        title_ = title;
    }

    const std::string& getDescription() const {
        return description_;
    }

    void setDescription(const std::string& description) {
        description_ = description;
    }

    bool isCompleted() const {
        return completed_;
    }

    void setCompleted(bool completed) {
        completed_ = completed;
    }

private:
    int id_;
    std::string title_;
    std::string description_;
    bool completed_;
};
    