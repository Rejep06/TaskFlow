#include "console_app.hpp"

#include <httplib.h>

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ConsoleApp::ConsoleApp(const std::string& baseUrl)
    : baseUrl_(baseUrl) {}

void ConsoleApp::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleApp::printMenu() {
    std::cout << "\nМеню:\n";
    std::cout << "1. Создать задачу\n";
    std::cout << "2. Показать все задачи\n";
    std::cout << "3. Редактировать задачу\n";
    std::cout << "4. Переключить статус\n";
    std::cout << "5. Удалить\n";
    std::cout << "6. Показать просроченные\n";
    std::cout << "7. Выход\n";
    std::cout << "Выберите пункт: ";
}
std::atomic<bool> running_notification{true};
void ConsoleApp::notificationLoop() {
    httplib::Client cli(baseUrl_);
    while (running_notification) {
        auto res = cli.Get("/notifications");
        if (res && res->status == 200) {
            auto arr = json::parse(res->body);
            for (const auto& n : arr) {
                std::cout << "\n🔔 УВЕДОМЛЕНИЕ\n";
                std::cout << "Задача: " << n["title"] << "\n";
                std::cout << n["message"] << "\n";
                std::cout << "------------------\n";
            }
        }
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }
}

void ConsoleApp::run() {
    auto notifier_ = std::thread(&ConsoleApp::notificationLoop, this);
    httplib::Client client(baseUrl_);
    bool running = true;
    while (running) {
        printMenu();

        int choice;
        if (!(std::cin >> choice)) {
            clearInput();
            continue;
        }
        clearInput();

        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                listTasks();
                break;
            case 3:
                editTask();
                break;
            case 4:
                toggleTaskStatus();
                break;
            case 5:
                deleteTask();
                break;
            case 6:
                showOverdueTasks();
                break;
            case 7:
                running = false;
                break;
            default:
                std::cout << "Нет такого пункта\n";
        }
    }
    running_notification = false;
    if (notifier_.joinable()) {
        notifier_.join();
    }
}

void ConsoleApp::addTask() {
    httplib::Client cli(baseUrl_);

    std::string title, description, deadline;

    std::cout << "Заголовок: ";
    std::getline(std::cin, title);

    std::cout << "Описание: ";
    std::getline(std::cin, description);

    std::cout << "Дедлайн (ISO или пусто): ";
    std::getline(std::cin, deadline);

    json body = {
        {"title", title},
        {"description", description}};

    if (!deadline.empty()) {
        body["deadline"] = deadline;
    }

    auto res = cli.Post("/tasks", body.dump(), "application/json");

    if (!res) {
        std::cout << "Ошибка подключения к серверу\n";
        return;
    }

    std::cout << "Ответ сервера:\n"
              << res->body << "\n";
}

void ConsoleApp::listTasks() {
    httplib::Client cli(baseUrl_);

    auto res = cli.Get("/tasks");

    if (!res) {
        std::cout << "Ошибка подключения\n";
        return;
    }

    json tasks = json::parse(res->body);

    std::cout << "\n=== Задачи ===\n";
    for (auto& t : tasks) {
        std::cout << "ID: " << t["id"] << "\n";
        std::cout << "Заголовок: " << t["title"] << "\n";
        std::cout << "Описание: " << t["description"] << "\n";
        std::cout << "Дедлайн: " << t["deadline"] << "\n";
        std::cout << "Статус: " << (t["completed"].get<bool>() ? "ВЫПОЛНЕНА" : "НЕ ВЫПОЛНЕНА") << "\n";
        std::cout << "---------------------\n";
    }
}

void ConsoleApp::editTask() {
    httplib::Client cli(baseUrl_);

    int id;
    std::cout << "ID: ";
    if (!(std::cin >> id)) {
        clearInput();
        return;
    }
    clearInput();

    std::string title, description, deadline;

    std::cout << "Новый заголовок (enter — пропуск): ";
    std::getline(std::cin, title);

    std::cout << "Новое описание (enter — пропуск): ";
    std::getline(std::cin, description);

    std::cout << "Новый дедлайн (ISO / пусто): ";
    std::getline(std::cin, deadline);

    json body = json::object();

    if (!title.empty()) body["title"] = title;
    if (!description.empty()) body["description"] = description;
    if (!deadline.empty()) body["deadline"] = deadline;

    auto res = cli.Put(("/tasks/" + std::to_string(id)).c_str(),
                       body.dump(),
                       "application/json");

    if (!res) {
        std::cout << "Ошибка запроса\n";
        return;
    }

    std::cout << res->body << "\n";
}

void ConsoleApp::toggleTaskStatus() {
    httplib::Client cli(baseUrl_);

    int id;
    std::cout << "ID: ";
    if (!(std::cin >> id)) {
        clearInput();
        return;
    }
    clearInput();

    auto res = cli.Put(("/tasks/" + std::to_string(id) + "/toggle").c_str(),
                       "",
                       "application/json");

    if (!res) {
        std::cout << "Ошибка запроса\n";
        return;
    }

    std::cout << res->body << "\n";
}

void ConsoleApp::deleteTask() {
    httplib::Client cli(baseUrl_);

    int id;
    std::cout << "ID: ";
    if (!(std::cin >> id)) {
        clearInput();
        return;
    }
    clearInput();

    auto res = cli.Delete(("/tasks/" + std::to_string(id)).c_str());

    if (!res) {
        std::cout << "Ошибка запроса\n";
        return;
    }

    std::cout << res->body << "\n";
}

void ConsoleApp::showOverdueTasks() {
    httplib::Client cli(baseUrl_);

    auto res = cli.Get("/tasks/overdue");

    if (!res) {
        std::cout << "Ошибка подключения\n";
        return;
    }

    json tasks = json::parse(res->body);

    std::cout << "\n=== Просроченные задачи ===\n";
    for (const auto& t : tasks) {
        std::cout << "ID: " << t["id"] << " | "
                  << t["title"] << " | "
                  << t["deadline"] << "\n";
    }
}
