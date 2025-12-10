#include <iostream>

#include "console_app.hpp"
#include "task_manager.hpp"

// Функция настройки консоли
void setupConsole() {
#ifdef _WIN32
    // Windows: UTF-8 и очистка буфера
    system("chcp 65001 > nul");
    system("cls");
#else
    // Unix/Linux/Mac: UTF-8 локаль
    try {
        std::locale::global(std::locale("C.UTF-8"));
    } catch (...) {
        try {
            std::locale::global(std::locale("en_US.UTF-8"));
        } catch (...) {
            // Оставляем по умолчанию
        }
    }
    system("clear");
#endif
}

int main() {
    setupConsole();

    std::cout << "=====================================\n";
    std::cout << "     Task Tracker v1.0\n";
    std::cout << "=====================================\n\n";

    try {
        ConsoleApp app("http://localhost:8080");
        app.run();
        std::cout << "\nСпасибо за использование Task Tracker!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n[ОШИБКА] " << e.what() << std::endl;
        std::cerr << "Нажмите Enter для выхода...";
        std::cin.get();
        return 1;
    }
}