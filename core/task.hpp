#pragma once
#include <string>

class Task {
   public:
    Task(int id,
         const std::string& title,
         const std::string& description,
         bool completed = false);

    int getId() const;

    const std::string& getTitle() const;

    void setTitle(const std::string& title);

    const std::string& getDescription() const;

    void setDescription(const std::string& description);

    bool isCompleted() const;

    void setCompleted(bool completed);

   private:
    int id_;
    std::string title_;
    std::string description_;
    bool completed_;
};
