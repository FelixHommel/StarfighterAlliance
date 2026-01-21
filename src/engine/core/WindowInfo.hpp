#ifndef SFA_SRC_ENGINE_CORE_WINDOW_INFO_HPP
#define SFA_SRC_ENGINE_CORE_WINDOW_INFO_HPP

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <memory>
#include <string>

namespace sfa
{

/// \brief Contains information about the window
///
/// \author Felix Hommel
/// \date Nov 20, 2024
struct WindowInfo
{
    int width;
    int height;
    int refreshRate;
    std::string title;
    std::shared_ptr<GLFWwindow> window;
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_WINDOW_INFO_HPP

