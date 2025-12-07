#include "task_manager.hpp"
#include "console_app.hpp"

int main() {
    TaskManager manager;
    ConsoleApp app(manager);
    app.run();
}
