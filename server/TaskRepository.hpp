#pragma once
#include <memory>
#include <vector>

#include "task.hpp"
#include "utils/ParserTime.hpp"

class TaskRepository {
   public:
    TaskRepository(std::shared_ptr<ParserTime> p);
    std::vector<Task> load(const std::string& username);
    void save(const std::string& username, const std::vector<Task>& tasks);

   private:
    std::shared_ptr<ParserTime> parserTime;
    
    std::string getFilename(const std::string& username) const {
        return "data/" + username + "_tasks.json";
    }
};
