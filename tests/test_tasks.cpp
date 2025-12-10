#include <gtest/gtest.h>
#include "../core/task_manager.cpp"

TEST(TaskManager, AddTask) {
    int id = addTask("Test task");
    ASSERT_EQ(id, 1);
}
