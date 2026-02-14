#include "ThreadPool.hpp"

#include <cstddef>
#include <exception>
#include <functional>
#include <mutex>
#include <queue>
#include <stop_token>
#include <utility>

#include <spdlog/spdlog.h>

namespace sfa
{

ThreadPool::ThreadPool(std::size_t nThreads)
{
    m_workers.reserve(nThreads);

    for(auto i{ 0 }; i < nThreads; ++i)
        m_workers.emplace_back([this](std::stop_token st) { worker(st); });
}

ThreadPool::~ThreadPool()
{
    shutdown();
}

void ThreadPool::shutdown(bool drainQueue) noexcept
{
    if(drainQueue)
    {
        {
            std::lock_guard lock(m_mutex);
            m_draining = true;
        }

        std::unique_lock lock(m_mutex);
        m_drainingComplete.wait(lock, [this]() {
            return m_queue.empty();
        });
    }
    else
    {
        std::lock_guard lock(m_mutex);

        // NOTE: Easy way to clear the queue, canceling pending jobs
        std::queue<std::function<void()>> emptyQueue;
        std::swap(m_queue, emptyQueue);
    }

    for(auto& worker : m_workers)
        worker.request_stop();

    m_signal.notify_all();

    // NOTE: join by destruction. Allows currently running tasks to finish running
    m_workers.clear();
}

/// \brief Wrapper function that invokes the enqueued tasks
void ThreadPool::worker(std::stop_token st)
{
    for(;;)
    {
        std::function<void()> job;

        {
            std::unique_lock lock(m_mutex);
            m_signal.wait(lock, [&]() {
                return st.stop_requested() || !m_queue.empty();
            });

            // NOTE:
            // If queue is in draining mode, finish the current queue
            // If not in draining mode and stopped, exit immediately
            if(st.stop_requested() && (m_queue.empty() || !m_draining))
            {
                if(m_queue.empty())
                    m_drainingComplete.notify_all();

                return;
            }

            if(m_queue.empty())
                continue;

            job = std::move(m_queue.front());
            m_queue.pop();

            // NOTE: Notify drain completion checker if the queue became empty
            if(m_queue.empty() && m_draining)
                m_drainingComplete.notify_all();
        }

        try
        {
            job();
        }
        catch(const std::exception& e)
        {
            spdlog::error("ThreadPool worker caught exception: {}", e.what());
        }
        catch(...)
        {
            spdlog::error("ThreadPool worker caught unknown exception");
        }
    }
}

} // namespace sfa

