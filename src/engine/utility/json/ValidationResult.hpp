#ifndef SFA_SRC_ENGINE_UTILITY_JSON_VALIDATION_RESULT_HPP
#define SFA_SRC_ENGINE_UTILITY_JSON_VALIDATION_RESULT_HPP

#include <valijson/validation_results.hpp>

namespace sfa::json::validation
{

struct ValidationResult
{
    bool isValid{};
    valijson::ValidationResults validationErrors;
};

} // namespace sfa::json::validation

#endif // !SFA_SRC_ENGINE_UTILITY_JSON_VALIDATION_RESULT_HPP

