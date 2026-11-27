#ifndef SFA_SRC_TESTS_TEST_UTILITY_COMPILER_WARNINGS_HPP
#define SFA_SRC_TESTS_TEST_UTILITY_COMPILER_WARNINGS_HPP

namespace sfa::testing
{

// NOTE: Macros to disable the -Wself-move warning (not available on MSVC)

#if defined(__clang__)

#    define DISABLE_SELF_MOVE_WARNING_BEGIN                                                  \
        _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wself-move\"")
#    define DISABLE_SELF_MOVE_WARNING_END _Pragma("clang diagnostic pop")

#elif defined(__GNUC__)

#    define DISABLE_SELF_MOVE_WARNING_BEGIN                                              \
        _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wself-move\"")
#    define DISABLE_SELF_MOVE_WARNING_END _Pragma("GCC diagnostic pop")

#else

#    define DISABLE_SELF_MOVE_WARNING_BEGIN
#    define DISABLE_SELF_MOVE_WARNING_END

#endif

} // namespace sfa::testing

#endif // !SFA_SRC_TESTS_TEST_UTILITY_COMPILER_WARNINGS_HPP

