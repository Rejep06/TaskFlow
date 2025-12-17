#include <httplib.h>

#include <iostream>

#include "TaskController.hpp"
#include "TaskRepository.hpp"
#include "task_manager.hpp"

int main() {
    std::shared_ptr<ParserTime> parserTime = std::make_shared<ParserTime>();
    std::shared_ptr<ITaskService> manager = std::make_shared<TaskManager>();
    std::shared_ptr<TaskRepository> repo = std::make_shared<TaskRepository>(parserTime);

    httplib::Server server;
    std::shared_ptr<TaskController> controller =
        std::make_shared<TaskController>(manager, repo, parserTime);
    controller->registerRoutes(server);

    std::cout << "=====================================\n";
    std::cout << "     Task Tracker Server v1.0\n";
    std::cout << "     Multi-user authorization enabled\n";
    std::cout << "=====================================\n";
    std::cout << "Server started on http://localhost:8080\n";
    std::cout << "Data will be saved in data/<username>_tasks.json\n\n";

    server.listen("0.0.0.0", 8080);
}
