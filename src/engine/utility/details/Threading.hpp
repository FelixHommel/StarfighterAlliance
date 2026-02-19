#ifndef SFA_SRC_ENGINE_UTILITY_DETAILS_THREADING_HPP
#define SFA_SRC_ENGINE_UTILITY_DETAILS_THREADING_HPP

#include <version>

#if !defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
#    define HAS_JTHREAD
#endif

#if defined(HAS_JTHREAD)
#    include <stop_token>
#    include <thread>
#else
#    include <atomic>
#    include <memory>
#    include <thread>
#    include <type_traits>
#    include <utility>
#endif

namespace sfa::threading
{

#if defined(HAS_JTHREAD)
constexpr bool HAS_NATIVE_STOP_TOKEN{ true };

using thread_t = std::jthread;
using stop_token_t = std::stop_token;
using stop_source_t = std::stop_source;
#else
constexpr bool HAS_NATIVE_STOP_TOKEN{ false };

/// \brief Class imitating \ref std::stop_token
///
/// \author Felix Hommel
/// \date 2/18/2026
class stop_token_t
{
public:
    stop_token_t() = default;

    [[nodiscard]] bool stop_requested() const noexcept { return m_flag && m_flag->load(std::memory_order_acquire); }

private:
    friend class stop_source_t;

    explicit stop_token_t(std::shared_ptr<std::atomic<bool>> flag) : m_flag(std::move(flag)) {}

    std::shared_ptr<std::atomic<bool>> m_flag;
};

/// \brief Class imitating \ref std::stop_source
///
/// \author Felix Hommel
/// \date 2/18/2026
class stop_source_t
{
public:
    stop_source_t() : m_flag(std::make_shared<std::atomic<bool>>(false)) {}

    [[nodiscard]] stop_token_t get_token() const noexcept { return stop_token_t{ m_flag }; }

    void request_stop() noexcept { m_flag->store(true, std::memory_order_release); }

private:
    std::shared_ptr<std::atomic<bool>> m_flag;
};

/// \brief Class imitating \ref std::jthread
///
/// \author Felix Hommel
/// \date 2/18/2026
class thread_t
{
public:
    thread_t() = default;

    template<typename F>
    // NOLINTNEXTLINE(bugprone-forwarding-reference-overload): thread_t should accept a rvalue function like object
    explicit thread_t(F&& f)
    {
        m_thread = std::thread([func = std::forward<F>(f), token = m_source.get_token()]() mutable {
            if constexpr(std::is_invocable_v<F&, stop_token_t>)
                func(token);
            else
                func();
        });
    }
    ~thread_t()
    {
        if(joinable())
            join();
    }

    thread_t& operator=(thread_t&& other) noexcept
    {
        if(this != &other)
        {
            if(joinable())
                join();

            m_thread = std::move(other.m_thread);
            m_source = std::move(other.m_source);
        }

        return *this;
    }

    thread_t(const thread_t&) = delete;
    thread_t& operator=(const thread_t&) = delete;
    thread_t(thread_t&&) noexcept = default;

    [[nodiscard]] bool joinable() const noexcept { return m_thread.joinable(); }
    void join() { m_thread.join(); }

    void request_stop() noexcept { m_source.request_stop(); }
    [[nodiscard]] stop_token_t get_stop_token() const noexcept { return m_source.get_token(); }

private:
    std::thread m_thread;
    stop_source_t m_source;
};
#endif

} // namespace sfa::threading

#endif // SFA_SRC_ENGINE_UTILITY_DETAILS_THREADING_HPP

