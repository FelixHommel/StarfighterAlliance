#ifndef SFA_SRC_ENGINE_UTILITY_EXCEPTIONS_RESOURCE_EXCEPTION_HPP
#define SFA_SRC_ENGINE_UTILITY_EXCEPTIONS_RESOURCE_EXCEPTION_HPP

#include "utility/exceptions/Exception.hpp"

#include <fmt/format.h>

#include <source_location>
#include <string>
#include <utility>

namespace sfa
{

/// \brief Exception to be used when window creation failed.
///
/// \author Felix Hommel
/// \date 2/23/2026
class WindowCreationException : public Exception
{
public:
    explicit WindowCreationException(std::string message, std::source_location location = std::source_location::current())
        : Exception(std::move(message), std::move(location))
    {}
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_EXCEPTIONS_RESOURCE_EXCEPTION_HPP

