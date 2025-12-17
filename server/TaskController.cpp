#include "TaskController.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TaskController::TaskController(std::shared_ptr<ITaskService> m,
                               std::shared_ptr<TaskRepository> r,
                               std::shared_ptr<ParserTime> p)
    : manager(m), repo(r), parserTime(p) {}

void TaskController::registerRoutes(httplib::Server& server) {
    // Логин
    server.Post("/login", [&](const httplib::Request& req, httplib::Response& res) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"invalid json\"}", "application/json");
            return;
        }
        
        if (!body.contains("username") || body["username"].get<std::string>().empty()) {
            res.status = 400;
            res.set_content("{\"error\":\"Username required\"}", "application/json");
            return;
        }
        
        std::string username = body["username"].get<std::string>();
        std::cout << "👤 Пользователь " << username << " вошёл в систему\n";
        
        res.set_content("{\"status\":\"ok\", \"message\":\"Welcome " + username + "\"}", "application/json");
    });
    
    // Получить задачи пользователя
    server.Post("/tasks/list", [&](const httplib::Request& req, httplib::Response& res) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"invalid json\"}", "application/json");
            return;
        }
        
        if (!body.contains("username") || body["username"].get<std::string>().empty()) {
            res.status = 400;
            res.set_content("{\"error\":\"Username required\"}", "application/json");
            return;
        }
        
        std::string username = body["username"].get<std::string>();
        auto tasks = manager->getAllTasks(username);
        
        json j = json::array();
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
    
    // Создать задачу
    server.Post("/tasks", [&](const httplib::Request& req, httplib::Response& res) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"invalid json\"}", "application/json");
            return;
        }
        
        if (!body.contains("username") || body["username"].get<std::string>().empty()) {
            res.status = 400;
            res.set_content("{\"error\":\"Username required\"}", "application/json");
            return;
        }
        
        if (!body.contains("title") || !body.contains("description")) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing fields\"}", "application/json");
            return;
        }
        
        std::string username = body["username"].get<std::string>();
        std::optional<std::chrono::system_clock::time_point> deadline;
        
        if (body.contains("deadline") && !body["deadline"].is_null()) {
            std::string d = body["deadline"].get<std::string>();
            if (!d.empty()) {
                deadline = parserTime->stringToTimePoint(d);
            }
        }
        
        Task& task = manager->createTask(
            username,
            body["title"].get<std::string>(),
            body["description"].get<std::string>(),
            deadline);
        
        // Сохраняем все задачи пользователя
        auto userTasks = manager->getAllTasks(username);
        repo->save(username, userTasks);
        
        json response = {
            {"status", "created"},
            {"id", task.getId()},
            {"message", "Task created successfully"}};
        
        res.set_content(response.dump(), "application/json");
    });
    
    // Обновить задачу
    server.Put("/tasks", [&](const httplib::Request& req, httplib::Response& res) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"invalid json\"}", "application/json");
            return;
        }
        
        if (!body.contains("username") || body["username"].get<std::string>().empty()) {
            res.status = 400;
            res.set_content("{\"error\":\"Username required\"}", "application/json");
            return;
        }
        
        if (!body.contains("id")) {
            res.status = 400;
            res.set_content("{\"error\":\"Task ID required\"}", "application/json");
            return;
        }
        
        std::string username = body["username"].get<std::string>();
        int id = body["id"].get<int>();
        
        Task* task = manager->findTaskById(username, id);
        if (!task) {
            res.status = 404;
            res.set_content("{\"error\":\"Task not found\"}", "application/json");
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
        
        // Сохраняем все задачи пользователя
        auto userTasks = manager->getAllTasks(username);
        repo->save(username, userTasks);
        
        res.set_content("{\"status\":\"updated\"}", "application/json");
    });
    
    // Удалить задачу
    server.Delete("/tasks", [&](const httplib::Request& req, httplib::Response& res) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"invalid json\"}", "application/json");
            return;
        }
        
        if (!body.contains("username") || body["username"].get<std::string>().empty()) {
            res.status = 400;
            res.set_content("{\"error\":\"Username required\"}", "application/json");
            return;
        }
        
        if (!body.contains("id")) {
            res.status = 400;
            res.set_content("{\"error\":\"Task ID required\"}", "application/json");
            return;
        }
        
        std::string username = body["username"].get<std::string>();
        int id = body["id"].get<int>();
        
        if (manager->deleteTask(username, id)) {
            auto userTasks = manager->getAllTasks(username);
            repo->save(username, userTasks);
            res.set_content("{\"status\":\"deleted\"}", "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"error\":\"Task not found\"}", "application/json");
        }
    });
    
    // Переключить статус задачи
    server.Put("/tasks/toggle", [&](const httplib::Request& req, httplib::Response& res) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"invalid json\"}", "application/json");
            return;
        }
        
        if (!body.contains("username") || body["username"].get<std::string>().empty()) {
            res.status = 400;
            res.set_content("{\"error\":\"Username required\"}", "application/json");
            return;
        }
        
        if (!body.contains("id")) {
            res.status = 400;
            res.set_content("{\"error\":\"Task ID required\"}", "application/json");
            return;
        }
        
        std::string username = body["username"].get<std::string>();
        int id = body["id"].get<int>();
        
        Task* task = manager->findTaskById(username, id);
        if (!task) {
            res.status = 404;
            res.set_content("{\"error\":\"Task not found\"}", "application/json");
            return;
        }
        
        task->setCompleted(!task->isCompleted());
        
        auto userTasks = manager->getAllTasks(username);
        repo->save(username, userTasks);
        
        res.set_content("{\"status\":\"toggled\"}", "application/json");
    });
    
    // Просроченные задачи
    server.Post("/tasks/overdue", [&](const httplib::Request& req, httplib::Response& res) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content("{\"error\":\"invalid json\"}", "application/json");
            return;
        }
        
        if (!body.contains("username") || body["username"].get<std::string>().empty()) {
            res.status = 400;
            res.set_content("{\"error\":\"Username required\"}", "application/json");
            return;
        }
        
        std::string username = body["username"].get<std::string>();
        auto overdue = manager->getOverdueTasks(username);
        json j = json::array();
        
        for (auto* t : overdue) {
            json item = {
                {"id", t->getId()},
                {"title", t->getTitle()},
                {"description", t->getDescription()},
                {"deadline", parserTime->timePointToString(t->getDeadline())},
                {"completed", t->isCompleted()}};
            j.push_back(item);
        }
        
        res.set_content(j.dump(), "application/json");
    });
}
