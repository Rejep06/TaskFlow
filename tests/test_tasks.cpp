#include <gtest/gtest.h>
#include <chrono>

#include "../core/task_manager.hpp"

// Определим тестового пользователя для всех тестов
const std::string TEST_USER = "test_user";

// Тестируем создание задачи без дедлайна
TEST(TaskManagerTest, CreateTaskWithoutDeadline) {
    TaskManager manager;
    Task& task = manager.createTask(TEST_USER, "Task 1", "Description 1", std::nullopt);

    EXPECT_EQ(task.getId(), 1);
    EXPECT_EQ(task.getTitle(), "Task 1");
    EXPECT_EQ(task.getDescription(), "Description 1");
    EXPECT_EQ(task.getUsername(), TEST_USER);
    EXPECT_FALSE(task.isCompleted());

    const auto& allTasks = manager.getAllTasks(TEST_USER);
    EXPECT_EQ(allTasks.size(), 1);
}

// Тестируем создание задачи с дедлайном
TEST(TaskManagerTest, CreateTaskWithDeadline) {
    TaskManager manager;
    auto deadline = std::chrono::system_clock::now() + std::chrono::hours(24);
    Task& task = manager.createTask(TEST_USER, "Task 2", "Description 2", deadline);

    EXPECT_EQ(task.getId(), 1);
    EXPECT_EQ(task.getTitle(), "Task 2");
    EXPECT_EQ(task.getDescription(), "Description 2");
    EXPECT_EQ(task.getUsername(), TEST_USER);
    EXPECT_EQ(task.getDeadline(), deadline);

    const auto& allTasks = manager.getAllTasks(TEST_USER);
    EXPECT_EQ(allTasks.size(), 1);
}

// Тестируем поиск задачи по ID
TEST(TaskManagerTest, FindTaskById) {
    TaskManager manager;
    Task& task1 = manager.createTask(TEST_USER, "Task 1", "Desc 1", std::nullopt);
    Task& task2 = manager.createTask(TEST_USER, "Task 2", "Desc 2", std::nullopt);

    // Находим задачу правильного пользователя
    Task* foundTask = manager.findTaskById(TEST_USER, 2);
    ASSERT_NE(foundTask, nullptr);
    EXPECT_EQ(foundTask->getTitle(), "Task 2");
    EXPECT_EQ(foundTask->getUsername(), TEST_USER);

    // Проверяем, что другой пользователь не видит эту задачу
    Task* notFoundByOtherUser = manager.findTaskById("other_user", 2);
    EXPECT_EQ(notFoundByOtherUser, nullptr);

    // Несуществующая задача
    Task* notFound = manager.findTaskById(TEST_USER, 100);
    EXPECT_EQ(notFound, nullptr);
}

// Тестируем удаление задачи
TEST(TaskManagerTest, DeleteTask) {
    TaskManager manager;
    Task& task = manager.createTask(TEST_USER, "Task 1", "Desc", std::nullopt);

    bool deleted = manager.deleteTask(TEST_USER, task.getId());
    EXPECT_TRUE(deleted);
    EXPECT_TRUE(manager.getAllTasks(TEST_USER).empty());

    // Попытка удалить несуществующую задачу
    bool deleted2 = manager.deleteTask(TEST_USER, 100);
    EXPECT_FALSE(deleted2);
    
    // Попытка удалить задачу другого пользователя
    TaskManager manager2;
    Task& task2 = manager2.createTask("user1", "Task", "Desc", std::nullopt);
    bool deleted3 = manager2.deleteTask("user2", task2.getId());
    EXPECT_FALSE(deleted3);
}

// Тестируем получение просроченных задач
TEST(TaskManagerTest, OverdueTasks) {
    TaskManager manager;
    auto past = std::chrono::system_clock::now() - std::chrono::hours(1);
    auto future = std::chrono::system_clock::now() + std::chrono::hours(1);

    // Создаем просроченную задачу и оставляем невыполненной
    Task& t1 = manager.createTask(TEST_USER, "Overdue task", "desc", past);

    // Проверяем, что пока выполнена = false → должна попасть в overdue
    auto overdue = manager.getOverdueTasks(TEST_USER);
    ASSERT_EQ(overdue.size(), 1);
    EXPECT_EQ(overdue[0]->getTitle(), "Overdue task");
    EXPECT_EQ(overdue[0]->getUsername(), TEST_USER);

    // Теперь пометим как выполненную
    t1.setCompleted(true);
    overdue = manager.getOverdueTasks(TEST_USER);
    EXPECT_TRUE(overdue.empty());
    
    // Проверяем, что другой пользователь не видит просроченные задачи
    auto overdueOther = manager.getOverdueTasks("other_user");
    EXPECT_TRUE(overdueOther.empty());
}

// Тестируем изоляцию данных между пользователями
TEST(TaskManagerTest, UserIsolation) {
    TaskManager manager;
    
    // Пользователь 1 создает задачи
    Task& task1_user1 = manager.createTask("user1", "Task 1 User1", "Desc 1", std::nullopt);
    Task& task2_user1 = manager.createTask("user1", "Task 2 User1", "Desc 2", std::nullopt);
    
    // Пользователь 2 создает задачи
    Task& task1_user2 = manager.createTask("user2", "Task 1 User2", "Desc 1", std::nullopt);
    
    // Проверяем, что каждый пользователь видит только свои задачи
    EXPECT_EQ(manager.getAllTasks("user1").size(), 2);
    EXPECT_EQ(manager.getAllTasks("user2").size(), 1);
    EXPECT_EQ(manager.getAllTasks("user3").size(), 0);
    
    // Проверяем поиск с правильным пользователем
    Task* found = manager.findTaskById("user1", 1);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->getUsername(), "user1");
    EXPECT_EQ(found->getTitle(), "Task 1 User1");
    
    // Проверяем, что user2 не может найти задачи user1
    Task* notFound = manager.findTaskById("user2", 1);
    EXPECT_EQ(notFound, nullptr);
}

// Тестируем добавление существующей задачи
TEST(TaskManagerTest, AddExistingTask) {
    TaskManager manager;
    
    // Создаем задачу вручную и добавляем
    Task task(99, TEST_USER, "Existing Task", "Description", false);
    manager.addTask(task);
    
    const auto& tasks = manager.getAllTasks(TEST_USER);
    EXPECT_EQ(tasks.size(), 1);
    EXPECT_EQ(tasks[0].getId(), 99);
    EXPECT_EQ(tasks[0].getTitle(), "Existing Task");
    EXPECT_EQ(tasks[0].getUsername(), TEST_USER);
    
    // Следующая созданная задача должна получить корректный ID
    Task& newTask = manager.createTask(TEST_USER, "New Task", "Desc", std::nullopt);
    EXPECT_EQ(newTask.getId(), 100);
}

// Тестируем изменение статуса задачи
TEST(TaskManagerTest, ToggleTaskStatus) {
    TaskManager manager;
    Task& task = manager.createTask(TEST_USER, "Task", "Description", std::nullopt);
    
    EXPECT_FALSE(task.isCompleted());
    
    task.setCompleted(true);
    EXPECT_TRUE(task.isCompleted());
    
    // Проверяем, что в менеджере статус тоже изменился
    Task* found = manager.findTaskById(TEST_USER, task.getId());
    ASSERT_NE(found, nullptr);
    EXPECT_TRUE(found->isCompleted());
}