#pragma once

#include <iostream>
#include <string>
#include <limits>
#include "task_manager.hpp"

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
        std::cout << "Меню:" << "\n";
        std::cout << "1. Создать задачу\n";
        std::cout << "2. Показать все задачи\n";
        std::cout << "3. Редактировать задачу\n";
        std::cout << "4. Изменить статус задачи\n";
        std::cout << "5. Удалить задачу\n";
        std::cout << "6. Выход\n";
        std::cout << "Выберите пункт: ";
    }

    void listTasks() {
        const auto& tasks = manager_.getAllTasks();
        if (tasks.empty()) {
            std::cout << "Список задач пуст\n";
            return;
        }

        std::cout << "Текущие задачи:\n";
        std::cout << "Id\t" << "Заголовок\t\tОписание\t\tСтатус\n";
        for (const auto& task : tasks) {
            std::cout << task.getId() << "\t"
                      << task.getTitle() << "\t\t"
                      << (task.getDescription()).substr(0, 10) << "\t\t"
                      << (task.isCompleted() ? "выполнена\n" : "не выполнена\n") << "\n";
        }
    }

    void addTask() {
        std::string title;
        std::string description;

        std::cout << "Введите заголовок задачи: ";
        std::getline(std::cin, title);

        std::cout << "Введите описание задачи: ";
        std::getline(std::cin, description);

        Task& task = manager_.createTask(title, description);
        std::cout << "Задача создана, id: " << task.getId() << "\n";
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
        std::cout << "Новый статус: " << (newStatus ? "выполнена\n" : "не выполнена\n");
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
};
