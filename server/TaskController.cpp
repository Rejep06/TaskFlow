#include "TaskController.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TaskController::TaskController(std::shared_ptr<ITaskService> m,
                               std::shared_ptr<TaskRepository> r,
                               std::shared_ptr<ParserTime> p)
    : manager(m), repo(r), parserTime(p) {}

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

    server.Get(R"(/tasks/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        int id = std::stoi(req.matches[1]);
        Task* task = manager->findTaskById(id);

        if (!task) {
            res.status = 404;
            res.set_content("{\"error\":\"Task not found\"}", "application/json");
            return;
        }

        json j = {
            {"id", task->getId()},
            {"title", task->getTitle()},
            {"description", task->getDescription()},
            {"deadline", parserTime->timePointToString(task->getDeadline())},
            {"completed", task->isCompleted()}};

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

        if (body.contains("deadline") && !body["deadline"].is_null()) {
            std::string d = body["deadline"].get<std::string>();
            if (!d.empty()) {
                deadline = parserTime->stringToTimePoint(d);
            }
        }

        Task& task = manager->createTask(
            body["title"].get<std::string>(),
            body["description"].get<std::string>(),
            deadline);

        repo->save(manager->getAllTasks());

        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    server.Put(R"(/tasks/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        int id = std::stoi(req.matches[1]);
        Task* task = manager->findTaskById(id);

        if (!task) {
            res.status = 404;
            res.set_content("{\"error\":\"Task not found\"}", "application/json");
            return;
        }

        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"invalid json\"}", "application/json");
            return;
        }

        if (body.contains("title")) {
            task->setTitle(body["title"].get<std::string>());
        }

        if (body.contains("description")) {
            task->setDescription(body["description"].get<std::string>());
        }

        if (body.contains("deadline")) {
            if (body["deadline"].is_null()) {
                task->setDeadline(std::chrono::system_clock::time_point::max());
            } else {
                task->setDeadline(
                    parserTime->stringToTimePoint(body["deadline"].get<std::string>()));
            }
        }

        repo->save(manager->getAllTasks());
        res.set_content("{\"status\":\"updated\"}", "application/json");
    });

    server.Delete(R"(/tasks/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        int id = std::stoi(req.matches[1]);
        if (manager->deleteTask(id)) {
            repo->save(manager->getAllTasks());
            res.set_content("{\"status\":\"deleted\"}", "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"error\":\"Task not found\"}", "application/json");
        }
    });

    server.Put(R"(/tasks/(\d+)/toggle)", [&](const httplib::Request& req, httplib::Response& res) {
        int id = std::stoi(req.matches[1]);
        Task* task = manager->findTaskById(id);
        if (!task) {
            res.status = 404;
            res.set_content("{\"error\":\"Task not found\"}", "application/json");
            return;
        }
        task->setCompleted(!task->isCompleted());
        repo->save(manager->getAllTasks());
        res.set_content("{\"status\":\"toggled\"}", "application/json");
    });

    server.Get("/tasks/overdue", [&](const httplib::Request&, httplib::Response& res) {
        auto overdue = manager->getOverdueTasks();
        json j = json::array();

        for (auto* t : overdue) {
            json item = {{"id", t->getId()},
                         {"title", t->getTitle()},
                         {"description", t->getDescription()},
                         {"deadline", parserTime->timePointToString(t->getDeadline())},
                         {"completed", t->isCompleted()}};
            j.push_back(item);
        }

        res.set_content(j.dump(), "application/json");
    });

    server.Get("/notifications", [&](const httplib::Request&, httplib::Response& res) {
        json result = json::array();

        for (Task& task : manager->getAllTasks()) {
            auto type = task.checkNotification();
            if (!type.has_value()) continue;

            std::string msg;
            switch (*type) {
                case NotifyType::DAY:
                    msg = "Остался 1 день";
                    break;
                case NotifyType::HOUR:
                    msg = "Остался 1 час";
                    break;
                case NotifyType::MIN10:
                    msg = "Осталось 10 минут";
                    break;
                case NotifyType::DEADLINE:
                    msg = "Дедлайн наступил!";
                    break;
            }

            result.push_back({{"task_id", task.getId()},
                              {"title", task.getTitle()},
                              {"message", msg}});
        }

        res.set_content(result.dump(), "application/json");
    });
}
