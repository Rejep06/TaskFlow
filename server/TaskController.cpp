#include "TaskController.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

#include "ParserTime.hpp"

using json = nlohmann::json;


TaskController::TaskController(std::shared_ptr<ITaskService> m,
                               std::shared_ptr<TaskRepository> r,
                               std::shared_ptr<ParserTime> p)
    : manager(m), repo(r), parserTime(p) {
}

void TaskController::registerRoutes(httplib::Server& server) {
    server.Get("/tasks", [&](const httplib::Request&, httplib::Response& res) {
        auto tasks = manager->getAllTasks();
        json j;

        for (const auto& t : tasks) {
            json item = {
                {"id", t.getId()},
                {"title", t.getTitle()},
                {"description", t.getDescription()},
                {"deadline", parserTime->timePointToString(t.getDeadline())},
                {"completed", t.isCompleted()}};
            j.push_back(item);
        }

        res.set_content(j.dump(), "application/json");
    });

    server.Post("/tasks", [&](const httplib::Request& req, httplib::Response& res) {
        json body;

        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"invalid json\"}", "application/json");
            return;
        }

        if (!body.contains("title") || !body.contains("description")) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing fields\"}", "application/json");
            return;
        }

        std::optional<std::chrono::system_clock::time_point> deadline;

        if (body.contains("deadline"))
            deadline = parserTime->stringToTimePoint(body["deadline"].get<std::string>());

        Task& task = manager->createTask(
            body["title"].get<std::string>(),
            body["description"].get<std::string>(),
            deadline);

        repo->save(manager->getAllTasks());

        res.set_content("{\"status\":\"ok\"}", "application/json");
    });
}
