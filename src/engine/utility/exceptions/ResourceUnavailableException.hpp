#ifndef SFA_SRC_ENGINE_UTILITY_EXCEPTIONS_RESOURCE_EXCEPTION_HPP
#define SFA_SRC_ENGINE_UTILITY_EXCEPTIONS_RESOURCE_EXCEPTION_HPP

#include "utility/exceptions/Exception.hpp"

#include <fmt/format.h>

#include <source_location>
#include <string>
#include <utility>

namespace sfa
{

/// \brief Exception to be used when a resource is inaccessible.
///
/// \author Felix Hommel
/// \date 2/21/2026
class ResourceUnavailableException : public Exception
{
public:
    explicit ResourceUnavailableException(
        std::string message, std::string key, std::source_location location = std::source_location::current()
    )
        : Exception(std::move(message), std::move(location)), m_key(std::move(key))
    {
        ResourceUnavailableException::buildWhatMessage();
    }

protected:
    void buildWhatMessage() override
    {
        m_whatMessage = fmt::format(
            "ResourceException({} is not accessible): {}\n\tat {}: {} in {}",
            m_key,
            m_message,
            m_location.file_name(),
            m_location.line(),
            m_location.function_name()
        );
    }

private:
    std::string m_key;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_EXCEPTIONS_RESOURCE_EXCEPTION_HPP

