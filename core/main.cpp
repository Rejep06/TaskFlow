#include <windows.h>
#include <iostream>
#include <locale>
#include "console_app.hpp"

#include "task_manager.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        std::locale::global(std::locale("ru_RU.UTF-8"));
    } catch (...) {
        
    }

    TaskManager manager;
    ConsoleApp app(manager);
    app.run();
}
