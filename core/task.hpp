#pragma once
#include <string>

class Task {
   private:
    int id;
    std::string title;
    bool done;

   public:
    Task(int id = 0, const std::string& title = "", bool done = false)
        : id(id), title(title), done(done) {}
};