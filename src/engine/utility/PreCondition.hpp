#ifndef SFA_SRC_ENGINE_UTILITY_PRE_CONDITION_HPP
#define SFA_SRC_ENGINE_UTILITY_PRE_CONDITION_HPP

#include <utility>

namespace sfa::pre
{

/// \brief Ensure a condition is met, otherwise throw an exception.
///
/// \tparam ExceptionT The type of the exception that is thrown
/// \tparam Args The type of the arguments that are used to construct \ref ExceptionT
///
/// \param condition The condition that is checked
/// \param args The arguments that are used to construct an \ref ExceptionT
///
/// \throws A \ref ExceptionT if \p condition is false
template<typename ExceptionT, typename... Args>
void require(bool condition, Args&&... args)
{
    if(!condition)
        throw ExceptionT(std::forward<Args>(args)...);
}

} // namespace sfa::pre

#endif // !SFA_SRC_ENGINE_UTILITY_PRE_CONDITION_HPP

