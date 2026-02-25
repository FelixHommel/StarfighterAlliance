#ifndef SFA_SRC_ENGINE_UTILITY_EXCEPTIONS_EXCEPTION_HPP
#define SFA_SRC_ENGINE_UTILITY_EXCEPTIONS_EXCEPTION_HPP

#include <fmt/format.h>

#include <exception>
#include <source_location>
#include <string>
#include <utility>

namespace sfa
{

class Exception : public std::exception
{
public:
    explicit Exception(std::string message, std::source_location location = std::source_location::current())
        : m_message(std::move(message)), m_location(std::move(location))
    {
        Exception::buildWhatMessage();
    }

    [[nodiscard]] const char* what() const noexcept override { return m_whatMessage.c_str(); }
    [[nodiscard]] const std::source_location& location() const noexcept { return m_location; }

protected:
    std::string m_message;
    std::source_location m_location;
    std::string m_whatMessage;

    virtual void buildWhatMessage()
    {
        m_whatMessage = fmt::format(
            "Exception: {}\n\tat {}: {} in {}",
            m_message,
            m_location.file_name(),
            m_location.line(),
            m_location.function_name()
        );
    }
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_EXCEPTIONS_EXCEPTION_HPP

