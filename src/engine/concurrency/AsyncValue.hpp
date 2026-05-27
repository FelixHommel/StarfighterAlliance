#ifndef SFA_SRC_ENGINE_CONCURRENCY_ASYNC_VALUE_HPP
#define SFA_SRC_ENGINE_CONCURRENCY_ASYNC_VALUE_HPP

#include <atomic>
#include <mutex>
#include <type_traits>
#include <utility>

namespace sfa
{

/// \brief Wrapper class for values that are retrieved asynchronously
///
/// \tparam T type of the value. \p T needs to be default and move constructible.
///
/// \author Felix Hommel
/// \date 5/27/2026
template<typename T>
    requires std::is_default_constructible_v<T> && std::is_move_constructible_v<T>
class AsyncValue
{
public:
    /// \brief Construct a new \ref AsyncValue with a default value
    ///
    /// \param initValue (optional) provide a default value
    explicit AsyncValue(T initValue = {}) : m_value(std::move(initValue)) {}
    ~AsyncValue() = default;

    AsyncValue(const AsyncValue&) = delete;
    AsyncValue& operator=(const AsyncValue&) = delete;
    AsyncValue(AsyncValue&&) noexcept = delete;
    AsyncValue& operator=(AsyncValue&&) noexcept = delete;

    /// \brief Set the value
    ///
    /// \param value new value
    void set(T value)
    {
        {
            std::scoped_lock lock(m_mutex);
            m_value = std::move(value);
        }

        m_ready.store(true, std::memory_order_relaxed);
    }

    /// \brief Acquire the value or the default value if no value was set yet
    ///
    /// \returns current value
    [[nodiscard]] T get() const
    {
        std::scoped_lock lock(m_mutex);
        return m_value;
    }

    /// \brief Determine if the value has been set yet.
    ///
    /// \returns \p true if the value was set, \p false otherwise
    [[nodiscard]] bool ready() const { return m_ready.load(std::memory_order_acquire); }

private:
    mutable std::mutex m_mutex;
    T m_value;
    std::atomic_bool m_ready{ false };
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CONCURRENCY_ASYNC_VALUE_HPP
