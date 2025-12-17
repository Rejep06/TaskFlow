#include "console_app.hpp"

#include <httplib.h>

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ConsoleApp::ConsoleApp(const std::string& baseUrl)
    : baseUrl_(baseUrl), currentUsername_("") {}

bool ConsoleApp::login() {
    std::cout << "=== Добро пожаловать в Task Tracker! ===\n";
    std::cout << "Введите ваш никнейм: ";
    
    std::getline(std::cin, currentUsername_);
    
    if (currentUsername_.empty()) {
        std::cout << "Никнейм не может быть пустым!\n";
        return false;
    }
    
    httplib::Client cli(baseUrl_);
    json body = {{"username", currentUsername_}};
    auto res = cli.Post("/login", body.dump(), "application/json");
    
    if (!res) {
        std::cout << "Ошибка подключения к серверу\n";
        return false;
    }
    
    if (res->status == 200) {
        std::cout << "Привет, " << currentUsername_ << "!\n\n";
        return true;
    } else {
        std::cout << "Ошибка: " << res->body << "\n";
        return false;
    }
}

void ConsoleApp::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleApp::printMenu() {
    std::cout << "\nМеню пользователя: " << currentUsername_ << "\n";
    std::cout << "1. Создать задачу\n";
    std::cout << "2. Показать все задачи\n";
    std::cout << "3. Редактировать задачу\n";
    std::cout << "4. Переключить статус\n";
    std::cout << "5. Удалить\n";
    std::cout << "6. Показать просроченные\n";
    std::cout << "7. Выйти (сменить пользователя)\n";
    std::cout << "8. Завершить программу\n";
    std::cout << "Выберите пункт: ";
}

void ConsoleApp::run() {
    // Авторизация
    while (!login()) {
        // Повторять пока не войдёт
    }
    
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
                // Сменить пользователя
                while (!login()) {
                    // Повторять пока не войдёт
                }
                break;
            case 8:
                running = false;
                break;
            default:
                std::cout << "Нет такого пункта\n";
        }
    }
    
    std::cout << "До свидания, " << currentUsername_ << "!\n";
}

void ConsoleApp::addTask() {
    httplib::Client cli(baseUrl_);

    std::string title, description, deadline;

    std::cout << "Заголовок: ";
    std::getline(std::cin, title);

    std::cout << "Описание: ";
    std::getline(std::cin, description);

    std::cout << "Дедлайн (ГГГГ-ММ-ДД ЧЧ:ММ или пусто): ";
    std::getline(std::cin, deadline);

    json body = {
        {"username", currentUsername_},
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

    std::cout << "Ответ сервера: " << res->body << "\n";
}

void ConsoleApp::listTasks() {
    httplib::Client cli(baseUrl_);
    
    json body = {{"username", currentUsername_}};
    auto res = cli.Post("/tasks/list", body.dump(), "application/json");

    if (!res) {
        std::cout << "Ошибка подключения\n";
        return;
    }

    if (res->status != 200) {
        std::cout << "Ошибка сервера: " << res->body << "\n";
        return;
    }

    json tasks = json::parse(res->body);

    if (tasks.empty()) {
        std::cout << "\nУ вас пока нет задач.\n";
        return;
    }

    std::cout << "\n=== Ваши задачи (" << tasks.size() << ") ===\n";
    for (auto& t : tasks) {
        std::cout << "ID: " << t["id"] << "\n";
        std::cout << "Заголовок: " << t["title"] << "\n";
        std::cout << "Описание: " << t["description"] << "\n";
        std::string deadline = t["deadline"].get<std::string>();
        std::cout << "Дедлайн: " << (deadline.empty() ? "нет" : deadline) << "\n";
        std::cout << "Статус: " << (t["completed"].get<bool>() ? "✅ ВЫПОЛНЕНА" : "⏳ НЕ ВЫПОЛНЕНА") << "\n";
        std::cout << "---------------------\n";
    }
}

void ConsoleApp::editTask() {
    httplib::Client cli(baseUrl_);

    int id;
    std::cout << "ID задачи для редактирования: ";
    if (!(std::cin >> id)) {
        clearInput();
        std::cout << "Неверный ID\n";
        return;
    }
    clearInput();

    std::string title, description, deadline;

    std::cout << "Новый заголовок (Enter - оставить текущий): ";
    std::getline(std::cin, title);

    std::cout << "Новое описание (Enter - оставить текущее): ";
    std::getline(std::cin, description);

    std::cout << "Новый дедлайн (ГГГГ-ММ-ДД ЧЧ:ММ / Enter - оставить / 'нет' - удалить): ";
    std::getline(std::cin, deadline);

    json body = {
        {"username", currentUsername_},
        {"id", id}};

    if (!title.empty()) body["title"] = title;
    if (!description.empty()) body["description"] = description;
    if (!deadline.empty()) {
        if (deadline == "нет") {
            body["deadline"] = nullptr;
        } else {
            body["deadline"] = deadline;
        }
    }

    auto res = cli.Put("/tasks",
                       body.dump(),
                       "application/json");

    if (!res) {
        std::cout << "Ошибка запроса\n";
        return;
    }

    std::cout << "Результат: " << res->body << "\n";
}

void ConsoleApp::toggleTaskStatus() {
    httplib::Client cli(baseUrl_);

    int id;
    std::cout << "ID задачи: ";
    if (!(std::cin >> id)) {
        clearInput();
        std::cout << "Неверный ID\n";
        return;
    }
    clearInput();

    json body = {
        {"username", currentUsername_},
        {"id", id}};

    auto res = cli.Put("/tasks/toggle",
                       body.dump(),
                       "application/json");

    if (!res) {
        std::cout << "Ошибка запроса\n";
        return;
    }

    std::cout << "Результат: " << res->body << "\n";
}

void ConsoleApp::deleteTask() {
    httplib::Client cli(baseUrl_);

    int id;
    std::cout << "ID задачи для удаления: ";
    if (!(std::cin >> id)) {
        clearInput();
        std::cout << "Неверный ID\n";
        return;
    }
    clearInput();

    json body = {
        {"username", currentUsername_},
        {"id", id}};

    auto res = cli.Delete("/tasks", body.dump(), "application/json");

    if (!res) {
        std::cout << "Ошибка запроса\n";
        return;
    }

    std::cout << "Результат: " << res->body << "\n";
}

void ConsoleApp::showOverdueTasks() {
    httplib::Client cli(baseUrl_);
    
    json body = {{"username", currentUsername_}};
    auto res = cli.Post("/tasks/overdue", body.dump(), "application/json");

    if (!res) {
        std::cout << "Ошибка подключения\n";
        return;
    }

    if (res->status != 200) {
        std::cout << "Ошибка сервера: " << res->body << "\n";
        return;
    }

    json tasks = json::parse(res->body);

    if (tasks.empty()) {
        std::cout << "\nУ вас нет просроченных задач. Отлично! 👍\n";
        return;
    }

    std::cout << "\n=== Просроченные задачи (" << tasks.size() << ") ===\n";
    for (auto& t : tasks) {
        std::cout << "ID: " << t["id"] << " | "
                  << t["title"] << " | Дедлайн был: "
                  << t["deadline"].get<std::string>() << "\n";
    }
}