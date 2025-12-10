#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string>

#include "task_manager.hpp"

std::chrono::system_clock::time_point parseDateTime(const std::string& s) {
    std::tm tm = {};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
    if (ss.fail()) {
        throw std::invalid_argument("Неверный формат даты. Используйте: ГГГГ-ММ-ДД ЧЧ:ММ");
    }
    std::time_t time = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time);
}

std::string formatDateTime(std::chrono::system_clock::time_point tp) {
    if (tp == std::chrono::system_clock::time_point::max()) {
        return "без срока";
    }
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &time_t);
#else
    localtime_r(&time_t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return oss.str();
}

class ConsoleApp {
   public:
    explicit ConsoleApp(TaskManager& manager)
        : manager_(manager) {
    }

    void run() {
        bool is_running = true;
        while (is_running) {
            printMenu();
            int choice;
            if (!(std::cin >> choice)) {
                clearInput();
                std::cout << "Некорректный ввод, попробуйте снова\n";
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
                    is_running = false;
                    std::cout << "Выход из программы\n";
                    break;
                default:
                    std::cout << "Нет такого пункта меню\n";
                    break;
            }
        }
    }

   private:
    TaskManager& manager_;

    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void printMenu() {
        std::cout << "\n";
        std::cout << "Меню:\n";
        std::cout << "1. Создать задачу\n";
        std::cout << "2. Показать все задачи\n";
        std::cout << "3. Редактировать задачу\n";
        std::cout << "4. Изменить статус задачи\n";
        std::cout << "5. Удалить задачу\n";
        std::cout << "6. Показать просроченные задачи\n";
        std::cout << "7. Выход\n";
        std::cout << "Выберите пункт: ";
    }

    void listTasks() {
        const auto& tasks = manager_.getAllTasks();
        if (tasks.empty()) {
            std::cout << "Список задач пуст\n";
            return;
        }

        std::cout << "\nТекущие задачи:\n";
        std::cout << "Id\tЗаголовок\t\tОписание\t\tСтатус\t\tДедлайн\n";
        for (const auto& task : tasks) {
            std::cout
                << task.getId() << "\t"
                << task.getTitle() << "\t\t"
                << (task.getDescription().empty() ? "-" : task.getDescription().substr(0, 10)) << "\t\t"
                << (task.isCompleted() ? "выполнена" : "не выполнена") << "\t\t"
                << formatDateTime(task.getDeadline()) << "\n";
        }
    }

    void addTask() {
        std::string title;
        std::string description;
        std::string deadlineStr;

        std::cout << "Введите заголовок задачи: ";
        std::getline(std::cin, title);

        std::cout << "Введите описание задачи: ";
        std::getline(std::cin, description);

        std::cout << "Введите дедлайн (ГГГГ-ММ-ДД ЧЧ:ММ) или оставьте пустым (без срока): ";
        std::getline(std::cin, deadlineStr);

        if (deadlineStr.empty()) {
            Task& task = manager_.createTask(title, description, std::optional<std::chrono::system_clock::time_point>{});
            std::cout << "Задача создана, id: " << task.getId() << "\n";
        } else {
            try {
                auto deadline = parseDateTime(deadlineStr);
                Task& task = manager_.createTask(title, description, deadline);
                std::cout << "Задача с дедлайном создана, id: " << task.getId() << "\n";
            } catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << "\n";
                std::cout << "Задача создана без дедлайна.\n";
                Task& task = manager_.createTask(title, description, std::optional<std::chrono::system_clock::time_point>{});
                std::cout << "Id: " << task.getId() << "\n";
            }
        }
    }

    void editTask() {
        std::cout << "Введите id задачи для редактирования: ";
        int id;
        if (!(std::cin >> id)) {
            clearInput();
            std::cout << "Некорректный ввод id\n";
            return;
        }

        clearInput();
        Task* task = manager_.findTaskById(id);
        if (task == nullptr) {
            std::cout << "Задача с таким id не найдена\n";
            return;
        }

        std::cout << "Текущий заголовок: " << task->getTitle() << "\n";
        std::cout << "Введите новый заголовок (оставьте пустым, чтобы не менять): ";
        std::string newTitle;
        std::getline(std::cin, newTitle);
        if (!newTitle.empty()) {
            task->setTitle(newTitle);
        }

        std::cout << "Текущее описание: " << task->getDescription() << "\n";
        std::cout << "Введите новое описание (оставьте пустым, чтобы не менять): ";
        std::string newDescription;
        std::getline(std::cin, newDescription);
        if (!newDescription.empty()) {
            task->setDescription(newDescription);
        }

        std::cout << "Текущий дедлайн: " << formatDateTime(task->getDeadline()) << "\n";
        std::cout << "Введите новый дедлайн (ГГГГ-ММ-ДД ЧЧ:ММ) или оставьте пустым: ";
        std::string newDeadlineStr;
        std::getline(std::cin, newDeadlineStr);
        if (!newDeadlineStr.empty()) {
            try {
                auto newDeadline = parseDateTime(newDeadlineStr);
                task->setDeadline(newDeadline);
                std::cout << "Дедлайн обновлён.\n";
            } catch (const std::exception& e) {
                std::cout << "Ошибка при обновлении дедлайна: " << e.what() << "\n";
            }
        }

        std::cout << "Задача обновлена\n";
    }

    void toggleTaskStatus() {
        std::cout << "Введите id задачи для изменения статуса: ";
        int id;
        if (!(std::cin >> id)) {
            clearInput();
            std::cout << "Некорректный ввод id\n";
            return;
        }

        clearInput();
        Task* task = manager_.findTaskById(id);
        if (task == nullptr) {
            std::cout << "Задача с таким id не найдена\n";
            return;
        }

        bool newStatus = !task->isCompleted();
        task->setCompleted(newStatus);
        std::cout << "Новый статус: " << (newStatus ? "выполнена" : "не выполнена") << "\n";
    }

    void deleteTask() {
        std::cout << "Введите id задачи для удаления: ";
        int id;
        if (!(std::cin >> id)) {
            clearInput();
            std::cout << "Некорректный ввод id\n";
            return;
        }

        clearInput();
        if (manager_.deleteTask(id)) {
            std::cout << "Задача удалена\n";
        } else {
            std::cout << "Задача с таким id не найдена\n";
        }
    }

    void showOverdueTasks() {
        auto overdue = manager_.getOverdueTasks();
        if (overdue.empty()) {
            std::cout << "Нет просроченных задач.\n";
            return;
        }

        std::cout << "\n❗ Просроченные задачи:\n";
        for (Task* task : overdue) {
            std::cout
                << "Id: " << task->getId()
                << " | " << task->getTitle()
                << " | Дедлайн: " << formatDateTime(task->getDeadline()) << "\n";
        }
    }
};