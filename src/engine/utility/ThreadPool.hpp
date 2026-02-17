#ifndef SFA_SRC_ENGINE_UTILITY_THREAD_POOL_HPP
#define SFA_SRC_ENGINE_UTILITY_THREAD_POOL_HPP

#include "core/Utility.hpp"

#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace sfa
{

/// \brief A fixed size thread pool
///
/// Can be used to distribute workload onto a set amount of worker threads.
///
/// \author Felix Hommel
/// \date 2/7/2026
///
/// \see https://dev.to/ish4n10/making-a-thread-pool-in-c-from-scratch-bnm
class ThreadPool
{
public:
    /// \brief Create a new \ref ThreadPool
    ///
    /// \param nThreads how many threads the pool has available
    explicit ThreadPool(std::size_t nThreads);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    /// \brief Enqueue a new task for the pool
    ///
    /// \tparam F function-like object
    /// \tparam Args argument types for F
    ///
    /// \param f task that is enqueued
    /// \param args arguments required for \p f
    ///
    /// \returns result of \p f wrapped in a std::future
    template<typename F, typename... Args>
        requires std::is_invocable_v<F, Args...>
    auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
    {
        using R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>;

        auto task{ std::make_shared<std::packaged_task<R()>>(
            [func = std::decay_t<F>(std::forward<F>(f)),
             tup = std::tuple<std::decay_t<Args>...>(std::forward<Args>(args)...)]() mutable -> R {
                return std::apply(func, tup);
            }
        ) };

        auto future{ task->get_future() };

        {
            std::lock_guard lock(m_mutex);

            SFA_ASSERT(
                std::ranges::none_of(m_workers, [](const auto& w) { return w.get_stop_token().stop_requested(); }),
                "Can't enqueue on a stopped thread pool"
            );

            m_queue.emplace([task]() { (*task)(); });
        }

        m_signal.notify_one();

        return future;
    }

    /// \brief Shut down the thread pool
    ///
    /// \param drainQueue (optional) whether or not the queue should first drain the pending jobs or cancel them
    void shutdown(bool drainQueue = true) noexcept;

    [[nodiscard]] std::size_t pendingTasks() const noexcept
    {
        std::lock_guard lock(m_mutex);

        return m_queue.size();
    }

private:
    std::vector<std::jthread> m_workers;
    mutable std::mutex m_mutex;
    std::condition_variable m_signal;
    std::condition_variable m_drainingComplete;
    std::queue<std::function<void()>> m_queue;
    bool m_draining{ false };

    void worker(std::stop_token stopToken);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_THREAD_POOL_HPP

