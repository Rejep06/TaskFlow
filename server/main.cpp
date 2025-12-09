#include <httplib.h>

#include <iostream>

#include "TaskController.hpp"
#include "TaskManager.hpp"
#include "TaskRepository.hpp"

int main() {
    TaskRepository repo;
    TaskManager manager;

    auto tasks = repo.load();
    for (const auto& t : tasks) {
        manager.addTask(t);
    }

    httplib::Server server;
    TaskController controller(manager, repo);
    controller.registerRoutes(server);

    std::cout << "Server started on http://localhost:8080\n";
    server.listen("0.0.0.0", 8080);
}
