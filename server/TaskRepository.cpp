#include "TaskRepository.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "ParserTime.hpp"

using json = nlohmann::json;

TaskRepository::TaskRepository(std::shared_ptr<ParserTime> p)
    : parserTime(p) {
}

std::vector<Task> TaskRepository::load(const std::string& username) {
    std::vector<Task> tasks;
    std::string filename = getFilename(username);
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "ℹ️  Файл " << filename << " не найден. Создаём нового пользователя.\n";
        return tasks;
    }
    
    try {
        json j;
        file >> j;
        
        for (const auto& item : j) {
            Task task(
                item["id"].get<int>(),
                username,  // Берем из параметра, а не из файла
                item["title"].get<std::string>(),
                item["description"].get<std::string>(),
                parserTime->stringToTimePoint(item["deadline"].get<std::string>()),
                item["completed"].get<bool>());
            tasks.push_back(task);
        }
        
        std::cout << "✅ Загружено " << tasks.size() << " задач для пользователя " << username << "\n";
    } catch (const std::exception& e) {
        std::cerr << "❌ Ошибка загрузки файла " << filename << ": " << e.what() << std::endl;
    }
    
    return tasks;
}

void TaskRepository::save(const std::string& username, const std::vector<Task>& tasks) {
    std::filesystem::create_directories("data");
    std::string filename = getFilename(username);
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "❌ ERROR: Cannot open file: " << filename << std::endl;
        return;
    }

    json j = json::array();
    for (const auto& task : tasks) {
        if (task.getUsername() == username) {  // Сохраняем только задачи этого пользователя
            json item = {
                {"id", task.getId()},
                {"title", task.getTitle()},
                {"description", task.getDescription()},
                {"deadline", parserTime->timePointToString(task.getDeadline())},
                {"completed", task.isCompleted()}};
            j.push_back(item);
        }
    }

    file << j.dump(4);
    file.close();

    std::cout << "✅ Saved " << j.size() << " tasks for user " << username << " to " << filename << std::endl;
}