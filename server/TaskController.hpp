#pragma once
#include <httplib.h>

#include <memory>

#include "ITaskService.hpp"
#include "ParserTime.hpp"
#include "TaskRepository.hpp"

class TaskController {
   public:
    TaskController(std::shared_ptr<ITaskService> m, std::shared_ptr<TaskRepository> r, std::shared_ptr<ParserTime> p);
    void registerRoutes(httplib::Server& server);

   private:
    std::shared_ptr<ITaskService> manager;
    std::shared_ptr<TaskRepository> repo;
    std::shared_ptr<ParserTime> parserTime;
};
