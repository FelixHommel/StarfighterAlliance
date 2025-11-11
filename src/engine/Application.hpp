#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Game.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <memory>
#include <stdexcept>

/*
 * @file Application.hpp
 * @brief The Application class handles all the of the setup process
 *
 * @details The Application class handles all the GLFW Window setup process and sets up all the Callback
 *          prepares all of the functioanllity needed to work with OpenGL
 *
 * @author Felix Hommel
 * @date Nov 16, 2024
 */
class Application
{
	public:
		/** Constructor / Destructor */
		Application(int width = defaultWindowWidth, int height = defaultWindowHeight);
		~Application();

		/** Delete all copy constructors and assignment operators, Application objects shall not be copied */
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		/** Public member functions */
		void run();

		void framebufferResizeCallbackHandler(int width, int height);
		void cursorPosCallbackHandler(double posX, double posY);
		void mouseButtonCallbackHandler(bool isPressed);

	private:
		/** Private members */
		WindowInfo m_windowInfo;
		Mouse m_mouseInfo;

		std::unique_ptr<Game> m_game;
		std::shared_ptr<GLFWwindow> m_window;

		/** Default values */
		static constexpr auto defaultWindowWidth {960};
		static constexpr auto defaultWindowHeight {540};
		static constexpr auto applicationName {"Starfighter Alliance"};

		/** Private member functions */
		void initGLFW();
		void initGLAD();
		void initGLState() const;
		void registerCallbacks();
		void initApplication();

		void gameLoop();

		void processInput(float dt);
		void masterUpdate(float dt);
		void masterRender();
};

/** Callback definitons */
static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto* app {reinterpret_cast<Application*>(glfwGetWindowUserPointer(window))};
	app->framebufferResizeCallbackHandler(width, height);
}

static void cursorPosCallback(GLFWwindow* window, double posX, double posY)
{
	auto* app {reinterpret_cast<Application*>(glfwGetWindowUserPointer(window))};
	app->cursorPosCallbackHandler(posX, posY);
}

static void mouseButtonCallback(GLFWwindow* window, int /*button*/, int action, int /*mods*/)
{
	auto* app {reinterpret_cast<Application*>(glfwGetWindowUserPointer(window))};
	app->mouseButtonCallbackHandler(action == GLFW_PRESS);
}

/** Helper function to get GLFWwindow objects as shared_ptr */
static std::shared_ptr<GLFWwindow> createWindow(int width, int height, const std::string& title)
{
	GLFWwindow* window {glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)};

	if(window == nullptr)
		throw std::runtime_error("[ERROR] Failure while creating a window");

	return {window, [](GLFWwindow* window)
	{
		glfwDestroyWindow(window);
	}};
}

#endif //!APPLICATION_HPP
