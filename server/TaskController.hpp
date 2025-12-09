#pragma once
#include <httplib.h>

#include "ITaskService.hpp"
#include "TaskRepository.hpp"

class TaskController {
   public:
    TaskController(ITaskService& manager, TaskRepository& repo);
    void registerRoutes(httplib::Server& server);

   private:
    ITaskService& manager;
    TaskRepository& repo;
};
