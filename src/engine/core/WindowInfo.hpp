#ifndef CORE_WINDOW_INFO_HPP
#define CORE_WINDOW_INFO_HPP

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <memory>
#include <string>

/*
 * @brief Contains information about the window
 */
struct WindowInfo
{
    int width;
    int height;
    int refreshRate;
    std::string title;
    std::shared_ptr<GLFWwindow> window;
};

#endif //!CORE_WINDOW_INFO_HPP
