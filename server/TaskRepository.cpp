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

std::vector<Task> TaskRepository::load() {
    std::vector<Task> tasks;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return tasks;  // Return empty if file cannot be opened
    }
    json j;
    file >> j;
    for (const auto& item : j) {
        Task task(
            item["id"].get<int>(),
            item["title"].get<std::string>(),
            item["description"].get<std::string>(),
            parserTime->stringToTimePoint(item["deadline"].get<std::string>()),
            item["completed"].get<bool>());
        tasks.push_back(task);
    }
    return tasks;
}

void TaskRepository::save(const std::vector<Task>& tasks) {
    std::filesystem::create_directories("data");
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "❌ ERROR: Cannot open file: " << filename << std::endl;
        return;
    }

    json j;
    for (const auto& task : tasks) {
        json item = {
            {"id", task.getId()},
            {"title", task.getTitle()},
            {"description", task.getDescription()},
            {"deadline", parserTime->timePointToString(task.getDeadline())},
            {"completed", task.isCompleted()}};
        j.push_back(item);
    }

    file << j.dump(4);
    file.close();

    std::cout << "✅ Saved " << tasks.size() << " tasks to " << filename << std::endl;
}
