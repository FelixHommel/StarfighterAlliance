#include "Spaceship.hpp"

#include "GLFW/glfw3.h"

#include "utility/JSONReader.hpp"
#include "utility/ResourceManager.hpp"

namespace sfa
{

Spaceship::Spaceship(SpaceshipType type, ColorType color, const WindowInfo* windowInfo)
    : GameObject(windowInfo)
    , defaultPosition(
          glm::vec2(
              (static_cast<float>(m_windowInfo->width) * 0.5f) - (static_cast<float>(m_windowInfo->width) * 0.05f),
              static_cast<float>(m_windowInfo->height) - (static_cast<float>(m_windowInfo->height) * 0.15f)
          )
      )
    , defaultSize(
          glm::vec2(static_cast<float>(m_windowInfo->width) * 0.1f, static_cast<float>(m_windowInfo->height) * 0.1f)
      )
    , m_type(type)
    , m_speed(0)
    , m_shotPower(0.f)
    , m_shot(false)
{
    m_position = defaultPosition;
    m_size = defaultSize;

    m_movement = std::make_shared<MovementComponent>(&m_position, m_size, m_windowInfo->width, m_windowInfo->height);

    readDataFromFile();
    loadTexture(color);

    m_shooting = std::make_shared<ShootingComponent>(
        ResourceManager::getTexture("Projectile"), &m_position, m_shotPower, m_windowInfo
    );
}

void Spaceship::update(float dt)
{
    m_shooting->update(dt);

    if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_W) == GLFW_PRESS
       || glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_UP) == GLFW_PRESS)
        m_movement->moveY(-m_speed * movementSpeedFactor * dt);
    else if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_S) == GLFW_PRESS
            || glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_DOWN) == GLFW_PRESS)
        m_movement->moveY(m_speed * movementSpeedFactor * dt);

    if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_A) == GLFW_PRESS
       || glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_LEFT) == GLFW_PRESS)
        m_movement->moveX(-m_speed * movementSpeedFactor * dt);
    else if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_D) == GLFW_PRESS
            || glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        m_movement->moveX(m_speed * movementSpeedFactor * dt);

    if((glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_SPACE) == GLFW_PRESS) && !m_shot)
        m_shooting->shoot(m_shot);
    else if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_SPACE) == GLFW_RELEASE && m_shot)
        m_shot = false;
}

/*
 * @brief read the Properties of the Spaceship from a config file
 *
 * @throws std::runtime_error, when a value does not exist or thefile could not be opened
 */
void Spaceship::readDataFromFile()
{
    std::stringstream filepath;
    filepath << "resources/config/" << spaceshipTypeToString(m_type) << ".json";

    JSONReader file;
    if(file.open(filepath.str()))
    {
        auto val{ file.get<float>("speed") };
        if(val.has_value())
            m_speed = std::move(val.value());
        else
            throw std::runtime_error("[ERROR] value 'speed' could not be read out of the file: " + filepath.str());

        auto valInt{ file.get<int>("lives") };
        if(valInt.has_value())
            m_lives = std::move(valInt.value());
        else
            throw std::runtime_error("[ERROR] value 'lives' could not be read out of the file: " + filepath.str());

        val = file.get<int>("shotPower");
        if(val.has_value())
            m_shotPower = std::move(val.value());
        else
            throw std::runtime_error("[ERROR] value 'shotPower' could not be read out of the file: " + filepath.str());
    }
    else
        throw std::runtime_error(
            "[ERROR] could not open file or file is invalid.\n Path of the file: " + filepath.str()
        );
}

/*
 * @beif load the correct Texture for the spaceship
 *
 * @param color, the color of the spaceship
 */
void Spaceship::loadTexture(ColorType color)
{
    std::stringstream filepath;
    filepath << "resources/textures/" << spaceshipTypeToString(m_type) << "/" << spaceshipTypeToString(m_type) << "_"
             << Color::toString(color) << ".png";
    m_texture = ResourceManager::loadTexture(filepath.str().c_str(), "spaceship");
}

/*
 * @brief Convert a SpaceshipType::type to a std::string
 *
 * @param type, type to be converted
 */
std::string Spaceship::spaceshipTypeToString(SpaceshipType type)
{
    switch(type)
    {
    case SpaceshipType::x_wing:
        return "x_wing";
    case SpaceshipType::y_wing:
        return "y_wing";
    case SpaceshipType::millenium_falcon:
        return "millenium_falcon";
    }
}

} // namespace sfa

