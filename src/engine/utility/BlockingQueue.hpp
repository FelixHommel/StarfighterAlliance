#ifndef SFA_SRC_ENGINE_UTILITY_BLOCKING_QUEUE_HPP
#define SFA_SRC_ENGINE_UTILITY_BLOCKING_QUEUE_HPP

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

namespace sfa
{

/// \brief Simple thread-safe queue.
///
/// This is a thread-safe queue that is built around std::queue with synchronization around the critical operations.
///
/// \author Felix Hommel
/// \date 2/7/2026
///
/// \see https://gist.github.com/thelinked/6997598
template<typename T>
class BlockingQueue
{
public:
    BlockingQueue() = default;
    ~BlockingQueue() = default;

    BlockingQueue(const BlockingQueue&) = delete;
    BlockingQueue& operator=(const BlockingQueue&) = delete;
    BlockingQueue(BlockingQueue&&) = delete;
    BlockingQueue& operator=(BlockingQueue&&) = delete;

    /// \brief Push a new element to the queue.
    ///
    /// \param value the new element
    void push(T value)
    {
        {
            std::lock_guard lock(m_mutex);

            if(m_closed)
                return;

            m_queue.emplace(std::move(value));
        }

        m_signal.notify_one();
    }

    /// \brief Emplace a new element into the queue.
    ///
    /// \tparam Args Types of the parameters needed to construct a \p T
    /// \param args parameters to construct a \p T
    template<typename... Args>
    void emplace(Args&&... args)
    {
        {
            std::lock_guard lock(m_mutex);

            if(m_closed)
                return;

            m_queue.emplace(std::forward<Args>(args)...);
        }

        m_signal.notify_one();
    }

    /// \brief Close the queue.
    void close()
    {
        {
            std::lock_guard lock(m_mutex);

            m_closed = true;
        }

        m_signal.notify_all();
    }

    /// \brief Check if the queue is empty.
    ///
    /// \returns *true* if it is empty, *false* otherwise
    [[nodiscard]] bool empty() const
    {
        std::lock_guard lock(m_mutex);

        return m_queue.empty();
    }

    [[nodiscard]] std::size_t size() const
    {
        std::lock_guard lock(m_mutex);

        return m_queue.size();
    }

    /// \brief Try to take an element out of the queue.
    ///
    /// \returns optional containing the front queue value, empty optional if the queue is empty
    [[nodiscard]] std::optional<T> tryPop()
    {
        std::lock_guard lock(m_mutex);

        if(m_queue.empty())
            return std::nullopt;

        T v{ std::move(m_queue.front()) };
        m_queue.pop();

        return v;
    }

    /// \brief Try to take an element out of the queue. Wait if there is no element in the queue.
    ///
    /// \returns optional containing the front queue value, empty optional if the queue was closed while waiting for a value
    [[nodiscard]] std::optional<T> waitAndPop()
    {
        std::unique_lock lock(m_mutex);

        m_signal.wait(lock, [&]() { return m_closed || !m_queue.empty(); });

        if(m_queue.empty())
            return std::nullopt;

        T v{ std::move(m_queue.front()) };
        m_queue.pop();

        return v;
    }

private:
    std::queue<T> m_queue{};
    mutable std::mutex m_mutex;
    std::condition_variable m_signal;
    bool m_closed{ false };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_BLOCKING_QUEUE_HPP

