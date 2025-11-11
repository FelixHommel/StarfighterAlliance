#include "Application.hpp"

#include "internal/ResourceManager.hpp"

#include "glad/gl.h"

Application::Application(int width, int height) : m_windowInfo {width, height, 60, applicationName}, m_mouseInfo {}, m_game {nullptr} {}

Application::~Application()
{
	ResourceManager::clear();
	glfwTerminate();
}

/**
* The order of initialization is important.
*   1. GLFW -- Windowing
*   2. GLAD -- OpenGL Function loading
*   3. GLState -- OpenGL settings
*   4. Callbacks -- GLFW Callbacks
*   5. Application -- Prepare for running the game
*
*   After that the game can be started
*/
void Application::run()
{
	initGLFW();
	initGLAD();
	initGLState();
	registerCallbacks();
	initApplication();

	gameLoop();
}

/**
 * @param width, the new width of the framebuffer
 * @param height, the new height of the framebuffer
 * @return void
 */
void Application::framebufferResizeCallbackHandler(int width, int height)
{
	m_windowInfo.width = width;
	m_windowInfo.height = height;
	glViewport(0, 0, m_windowInfo.width, m_windowInfo.height);
}

/*
 * @param posX, current x-axis position of the cursor on the window
 * @param posY, current y-axis position of the cursor on the window
 * @return void
 */
void Application::cursorPosCallbackHandler(double posX, double posY)
{
	m_mouseInfo.posX = static_cast<float>(posX);
	m_mouseInfo.posY = static_cast<float>(posY);
}

/*
 * @param isPressed, whether or not a mousebutton is pressed
 * @return void
 */
void Application::mouseButtonCallbackHandler(bool isPressed)
{
	m_mouseInfo.isPressed = isPressed;
}

/*
 * @brief Initialize GLFW
 *
 * @throws runtime_error, if GLFW could not be initialized or window creation failed
 */
void Application::initGLFW()
{
	if(glfwInit() == 0)
		throw std::runtime_error("[ERROR] An error occured while initializing GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfwWindowHint(GLFW_RESIZABLE, 1);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vm = glfwGetVideoMode(monitor);
	m_windowInfo.width = vm->width;
	m_windowInfo.height = vm->height;
	m_windowInfo.refreshRate = vm->refreshRate;

	m_window = createWindow(m_windowInfo.width, m_windowInfo.height, m_windowInfo.title);
	glfwMakeContextCurrent(m_window.get());
	m_windowInfo.window = m_window;

	glfwSetWindowUserPointer(m_window.get(), this);
}

/*
 * @brief Initialize GLAD
 *
 * @throws runtime_error, if GLAD could not load the OpenGL functions
 */
void Application::initGLAD()
{
	if(gladLoadGL(glfwGetProcAddress) == 0)
		throw std::runtime_error("[ERROR] An error occured while initializing GLAD");
}

/*
 * @brief Initialize OpenGL
 */
void Application::initGLState() const
{
	glViewport(0, 0, static_cast<int>(m_windowInfo.width), static_cast<int>(m_windowInfo.height));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*
 * @brief Register Callback functions
 */
void Application::registerCallbacks()
{
	glfwSetFramebufferSizeCallback(m_window.get(), framebufferResizeCallback);
	glfwSetCursorPosCallback(m_window.get(), cursorPosCallback);
	glfwSetMouseButtonCallback(m_window.get(), mouseButtonCallback);
}

/*
 * @brief Initialize the Game object
 */
void Application::initApplication()
{
	m_game = std::make_unique<Game>(&m_windowInfo, &m_mouseInfo);
}

/*
 * @brief The main game loop of the Application
 *
 * @details In this function the main loop is executed. The deltatime is updated the user inputs are
 *          being processed, the masterUpdate() function is called, the screen is cleared and then
 *          the new scene is rendered with the masterRender() function.
 */
void Application::gameLoop()
{
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while(glfwWindowShouldClose(m_window.get()) == 0)
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        processInput(deltaTime);
        masterUpdate(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        masterRender();

        glfwSwapBuffers(m_window.get());
    }
}

/*
 * @brief within the masterUpdate() function, all other update functions are called
 *
 * @param dt, deltatime
 */
void Application::masterUpdate(float dt)
{
	m_game->update(dt);

	if(m_game->getQuit())
		glfwSetWindowShouldClose(m_window.get(), 1);
}

/*
 * @brief handle user inputs
 *
 * @param dt, deltatime
 */
void Application::processInput(float dt)
{
	if(glfwGetKey(m_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window.get(), 1);
}

/*
 * @brief within the masterRender() function, all otehr render functions are called
 */
void Application::masterRender()
{
	m_game->render();
}
