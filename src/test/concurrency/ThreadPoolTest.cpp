#include "concurrency/ThreadPool.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

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
TEST_CASE("ThreadPoolTest", "[unit][concurrency]")
{
    /// \brief Test the thread pool with multiple tasks.
    ///
    /// The thread pool should process every single task that it received.
    SECTION("Multiple tasks")
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
            CHECK(i == futures[i].get());

        CHECK(NUM_TASKS == counter);
    }

    /// \brief Test the thread pool shutdown with task drain enabled.
    ///
    /// When task drain is enabled, all tasks that were enqueued need to be finished before the pool can actually be closed
    /// while no new tasks can be submitted to the pool.
    SECTION("Shutdown drain")
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

        CHECK(completed == NUM_TASKS);
    }

    /// \brief Test the thread pool shutdown with task drain disabled.
    ///
    /// When task drain is disabled the tasks that were enqueued but were not yet able to run are canceled.
    SECTION("Shutdown cancel")
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
            futures.push_back(pool.enqueue([&completed]() { completed.fetch_add(1, std::memory_order_relaxed); }));
        }

        pool.shutdown(false);

        CHECK(completed.load() == NUM_TASKS_INITIAL);
    }

    /// \brief Test the case where a thread pool worker catches an exception thrown from the task.
    ///
    /// When a task throws an exception, the worker should catch it, log it, and then cancel itself. The future then should
    /// contain whatever exception was thrown.
    SECTION("Worker throws exception")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        constexpr std::size_t THREAD_POOL_SIZE{ 2 };
        constexpr auto ERROR_MESSAGE{ "error" };

        ThreadPool pool(THREAD_POOL_SIZE);

        auto f{ pool.enqueue([]() -> int {
            throw std::runtime_error(ERROR_MESSAGE);
            return 1;
        }) };

        CHECK_THROWS_MATCHES(f.get(), std::runtime_error, MessageMatches(ContainsSubstring(ERROR_MESSAGE)));
    }
}

} // namespace sfa::testing

