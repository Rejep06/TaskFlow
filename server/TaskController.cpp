#include "TaskController.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TaskController::TaskController(ITaskService& m, TaskRepository& r)
    : manager(m), repo(r) {}

void TaskController::registerRoutes(httplib::Server& server) {
    server.Get("/tasks", [&](const httplib::Request&, httplib::Response& res) {
        auto tasks = manager.getAllTasks();
        json j;

        for (const auto& t : tasks) {
            j.push_back({{"id", t.getId()},
                         {"title", t.getTitle()},
                         {"description", t.getDescription()},
                         {"deadline", t.getDeadline()},
                         {"completed", t.isCompleted()}});
        }

        res.set_content(j.dump(), "application/json");
    });

    server.Post("/tasks", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);

            Task task = manager.createTask(
                body["title"].get<std::string>(),
                body["description"].get<std::string>());

            repo.save(manager.getAllTasks());  // надо поменять на чтобы сохранял все задачи по необходимости

            res.set_content("{\"status\":\"ok\"}", "application/json");
        } catch (std::exception& e) {
            res.status = 500;
            res.set_content(std::string("{\"error\":\"") + e.what() + "\"}", "application/json");
            std::cerr << "POST /tasks error: " << e.what() << std::endl;
        }
    });
}
