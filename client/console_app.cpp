#include "console_app.hpp"
#include <ctime>

// Constructor
ConsoleApp::ConsoleApp(TaskManager& manager)
    : manager_(manager) {
}

// Helper functions
std::chrono::system_clock::time_point ConsoleApp::parseDateTime(const std::string& s) {
    std::tm tm = {};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
    if (ss.fail()) {
        throw std::invalid_argument("Неверный формат даты. Используйте: ГГГГ-ММ-ДД ЧЧ:ММ");
    }
    std::time_t time = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time);
}

std::string ConsoleApp::formatDateTime(std::chrono::system_clock::time_point tp) {
    if (tp == std::chrono::system_clock::time_point::max()) {
        return "no deadline";
    }
    
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm;
    
    #ifdef _WIN32
        // Windows безопасная версия
        localtime_s(&tm, &time_t);
    #else
        // Unix/Linux безопасная версия  
        localtime_r(&time_t, &tm);
    #endif
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return oss.str();
}

// Input/output utilities
void ConsoleApp::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleApp::printMenu() {
    std::cout << "\nМеню:\n";
    std::cout << "1. Создать задачу\n";
    std::cout << "2. Показать все задачи\n";
    std::cout << "3. Редактировать задачу\n";
    std::cout << "4. Изменить статус задачи\n";
    std::cout << "5. Удалить задачу\n";
    std::cout << "6. Показать просроченные задачи\n";
    std::cout << "7. Выход\n";
    std::cout << "Выберите пункт: ";
}

// Main loop
void ConsoleApp::run() {
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

// Menu handlers
void ConsoleApp::addTask() {
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

void ConsoleApp::listTasks() {
    const auto& tasks = manager_.getAllTasks();
    if (tasks.empty()) {
        std::cout << "Список задач пуст\n";
        return;
    }

    std::cout << "\n=== Текущие задачи (" << tasks.size() << ") ===\n\n";
    
    for (size_t i = 0; i < tasks.size(); i++) {
        const auto& task = tasks[i];
        
        std::cout << "[" << (i + 1) << "] Задача #" << task.getId() << "\n";
        std::cout << "    Заголовок: " << task.getTitle() << "\n";
        
        if (!task.getDescription().empty()) {
            std::cout << "    Описание: " << task.getDescription() << "\n";
        }
        
        std::cout << "    Статус: " 
                  << (task.isCompleted() ? "ВЫПОЛНЕНА" : "НЕ ВЫПОЛНЕНА") 
                  << "\n";
        
        std::string deadline = formatDateTime(task.getDeadline());
        std::cout << "    Дедлайн: " << deadline;
        
        if (task.isOverdue() && !task.isCompleted()) {
            std::cout << " (ПРОСРОЧЕНО!)";
        }
        
        std::cout << "\n";
        
        if (i < tasks.size() - 1) {
            std::cout << "\n";  // Пустая строка между задачами
        }
    }
    
    std::cout << "\n==============================\n";
}

void ConsoleApp::editTask() {
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

void ConsoleApp::toggleTaskStatus() {
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

void ConsoleApp::deleteTask() {
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

void ConsoleApp::showOverdueTasks() {
    auto overdue = manager_.getOverdueTasks();
    if (overdue.empty()) {
        std::cout << "Нет просроченных задач.\n";
        return;
    }

    std::cout << "\nПросроченные задачи (" << overdue.size() << "):\n";
    std::cout << "========================================\n\n";
    
    for (Task* task : overdue) {
        std::cout << "Id: " << task->getId() << "\n";
        std::cout << "Заголовок: " << task->getTitle() << "\n";
        std::cout << "Дедлайн был: " << formatDateTime(task->getDeadline()) << "\n";
        std::cout << "------------------------\n";
    }
}