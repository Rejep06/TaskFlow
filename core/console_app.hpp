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
                      << task.getDescription() << "\t\t"
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
        // TODO: Реализовать редактирование задачи
        std::cout << "Функция редактирования задачи (изменено)\n";
    }

    void toggleTaskStatus() {
        // TODO: Реализовать изменение статуса задачи
        std::cout << "Функция изменения статуса задачи (переключено)\n";
    }

    void deleteTask() {
        // TODO: Реализовать удаление задачи
        std::cout << "Функция удаления задачи (удалено)\n";
    }
};