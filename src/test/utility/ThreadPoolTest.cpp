#include "utility/ThreadPool.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <cstddef>
#include <future>
#include <latch>
#include <stdexcept>
#include <thread>
#include <vector>

namespace sfa::testing
{

/// \brief Test the features of the \ref ThreadPool.
///
/// \author Felix Hommel
/// \date 2/12/2026
class ThreadPoolTest : public ::testing::Test
{
public:
    ThreadPoolTest() = default;
    ~ThreadPoolTest() override = default;

    ThreadPoolTest(const ThreadPoolTest&) = delete;
    ThreadPoolTest& operator=(const ThreadPoolTest&) = delete;
    ThreadPoolTest(ThreadPoolTest&&) = delete;
    ThreadPoolTest& operator=(ThreadPoolTest&&) = delete;
};

/// \brief Test the thread pool with multiple tasks.
///
/// The thread pool should process every single task that it received.
TEST_F(ThreadPoolTest, MultipleTasks)
{
    constexpr std::size_t THREAD_POOL_SIZE{ 4 };
    constexpr auto NUM_TASKS{ 100 };

    ThreadPool pool(THREAD_POOL_SIZE);

    std::vector<std::future<int>> futures;
    std::atomic<int> counter{ 0 };

    for(int i{ 0 }; i < NUM_TASKS; ++i)
    {
        futures.push_back(pool.enqueue([&counter, i]() {
            counter.fetch_add(1, std::memory_order_relaxed);

            return i;
        }));
    }

    for(int i{ 0 }; i < NUM_TASKS; ++i)
        EXPECT_EQ(futures[i].get(), i);

    EXPECT_EQ(counter, NUM_TASKS);
}

/// \brief Test the thread pool shutdown with task drain enabled.
///
/// When task drain is enabled, all tasks that were enqueued need to be finished before the pool can actually be closed
/// while no new tasks can be submitted to the pool.
TEST_F(ThreadPoolTest, ShutdownDrain)
{
    constexpr std::size_t THREAD_POOL_SIZE{ 2 };
    constexpr auto NUM_TASKS{ 10 };
    constexpr auto SLEEP_TIME{ 50 };

    ThreadPool pool(THREAD_POOL_SIZE);

    std::vector<std::future<void>> futures;
    std::atomic<int> completed{ 0 };

    for(int i{ 0 }; i < NUM_TASKS; ++i)
    {
        futures.push_back(pool.enqueue([&completed, SLEEP_TIME]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

            completed.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    pool.shutdown(true);

    EXPECT_EQ(NUM_TASKS, completed);
}

/// \brief Test the thread pool shutdown with task drain disabled.
///
/// When task drain is disabled the tasks that were enqueued but were not yet able to run are canceled.
TEST_F(ThreadPoolTest, ShutdownCancel)
{
    constexpr std::size_t THREAD_POOL_SIZE{ 2 };
    constexpr auto NUM_TASKS_INITIAL{ 2 };
    constexpr auto NUM_TASKS_AFTERWARDS{ 20 };
    constexpr auto SLEEP_TIME{ 100 };

    ThreadPool pool(THREAD_POOL_SIZE);

    std::vector<std::future<void>> futures;
    std::atomic<int> completed{ 0 };
    std::latch startedTasks{ NUM_TASKS_INITIAL };

    for(int i{ 0 }; i < NUM_TASKS_INITIAL; ++i)
    {
        futures.push_back(pool.enqueue([&startedTasks, &completed, SLEEP_TIME]() {
            startedTasks.count_down();
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

            completed.fetch_add(1, std::memory_order_relaxed);
        }));
    }
    
    startedTasks.wait();

    for(int i{ 0 }; i < NUM_TASKS_AFTERWARDS; ++i)
    {
        futures.push_back(pool.enqueue([&completed]() {
            completed.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    pool.shutdown(false);

    EXPECT_EQ(NUM_TASKS_INITIAL, completed.load());
}

/// \brief Test the case where a thread pool worker catches an exception thrown from the task.
///
/// When a task throws an exception, the worker should catch it, log it, and then cancel itself. The future then should
/// contain whatever exception was thrown.
TEST_F(ThreadPoolTest, WorkerThrowsException)
{
    constexpr std::size_t THREAD_POOL_SIZE{ 2 };

    ThreadPool pool(THREAD_POOL_SIZE);

    auto f{ pool.enqueue([]() -> int { throw std::runtime_error("error"); return 1; }) };

    EXPECT_THROW({ f.get(); }, std::runtime_error);
}

} // namespace sfa::testing

