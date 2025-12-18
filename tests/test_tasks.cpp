#include <gtest/gtest.h>

#include "../core/task_manager.hpp"

// Тестируем создание задачи без дедлайна
TEST(TaskManagerTest, CreateTaskWithoutDeadline) {
    TaskManager manager;
    const Task& task = manager.createTask("Task 1", "Description 1", std::nullopt);

    EXPECT_EQ(task.getId(), 1);
    EXPECT_EQ(task.getTitle(), "Task 1");
    EXPECT_EQ(task.getDescription(), "Description 1");
    EXPECT_FALSE(task.isCompleted());

    const auto& allTasks = manager.getAllTasks();
    EXPECT_EQ(allTasks.size(), 1);
}

// Тестируем создание задачи с дедлайном
TEST(TaskManagerTest, CreateTaskWithDeadline) {
    TaskManager manager;
    auto deadline = std::chrono::system_clock::now() + std::chrono::hours(24);
    const Task& task = manager.createTask("Task 2", "Description 2", deadline);

    EXPECT_EQ(task.getId(), 1);
    EXPECT_EQ(task.getTitle(), "Task 2");
    EXPECT_EQ(task.getDescription(), "Description 2");
    EXPECT_EQ(task.getDeadline(), deadline);

    const auto& allTasks = manager.getAllTasks();
    EXPECT_EQ(allTasks.size(), 1);
}

// Тестируем поиск задачи по ID
TEST(TaskManagerTest, FindTaskById) {
    TaskManager manager;
    const Task& task1 = manager.createTask("Task 1", "Desc 1", std::nullopt);
    const Task& task2 = manager.createTask("Task 2", "Desc 2", std::nullopt);

    Task* foundTask = manager.findTaskById(2);
    ASSERT_NE(foundTask, nullptr);
    EXPECT_EQ(foundTask->getTitle(), "Task 2");

    Task* notFound = manager.findTaskById(100);
    EXPECT_EQ(notFound, nullptr);
}

// Тестируем удаление задачи
TEST(TaskManagerTest, DeleteTask) {
    TaskManager manager;
    const Task& task = manager.createTask("Task 1", "Desc", std::nullopt);

    bool deleted = manager.deleteTask(task.getId());
    EXPECT_TRUE(deleted);
    EXPECT_TRUE(manager.getAllTasks().empty());

    bool deleted2 = manager.deleteTask(100);
    EXPECT_FALSE(deleted2);
}

// Тестируем получение просроченных задач: в списке только невыполненные с прошедшим дедлайном
TEST(TaskManagerTest, OverdueTasks) {
    TaskManager manager;
    auto past = std::chrono::system_clock::now() - std::chrono::hours(1);
    auto future = std::chrono::system_clock::now() + std::chrono::hours(1);

    // Создаем просроченную задачу и оставляем невыполненной
    const Task& t1 = manager.createTask("Overdue task", "desc", past);

    auto overdue = manager.getOverdueTasks();
    ASSERT_EQ(overdue.size(), 1);
    EXPECT_EQ(overdue[0]->getTitle(), "Overdue task");

    Task* overdueTask = manager.findTaskById(overdueId);
    ASSERT_NE(overdueTask, nullptr);
    overdueTask->setCompleted(true);
    overdue = manager.getOverdueTasks();
    EXPECT_TRUE(overdue.empty());
}

// Тестируем кандидатов на уведомления: только просроченные, невыполненные и с реальным дедлайном
TEST(TaskManagerTest, OverdueTasksAreNotificationCandidates) {
    TaskManager manager;
    auto past = std::chrono::system_clock::now() - std::chrono::hours(3);
    auto future = std::chrono::system_clock::now() + std::chrono::hours(3);

    int overdueId = manager.createTask("Overdue", "desc", past).getId();
    manager.createTask("Upcoming", "desc", future);
    manager.createTask("No deadline", "desc", std::nullopt);

    auto candidates = manager.getOverdueTasks();
    ASSERT_EQ(candidates.size(), 1);
    EXPECT_EQ(candidates[0]->getId(), overdueId);

    Task* overdueTask = manager.findTaskById(overdueId);
    ASSERT_NE(overdueTask, nullptr);
    overdueTask->setCompleted(true);
    candidates = manager.getOverdueTasks();
    EXPECT_TRUE(candidates.empty());
}
