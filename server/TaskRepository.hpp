#pragma once
#include <memory>
#include <vector>

#include "task.hpp"
#include "utils/ParserTime.hpp"

class TaskRepository {
   public:
    TaskRepository(std::shared_ptr<ParserTime> p);
    std::vector<Task> load();
    void save(const std::vector<Task>& tasks);

   private:
    std::shared_ptr<ParserTime> parserTime;
    const std::string filename = "data/tasks.json";
};
