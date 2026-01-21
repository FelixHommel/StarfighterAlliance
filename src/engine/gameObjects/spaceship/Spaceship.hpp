#ifndef GAME_OBJECTS_SPACESHIP_SPACESHIP_HPP
#define GAME_OBJECTS_SPACESHIP_SPACESHIP_HPP

#include "core/Color.hpp"
#include "gameObjects/GameObject.hpp"
#include "gameObjects/MovementComponent.hpp"
#include "gameObjects/ShootingComponent.hpp"

namespace sfa
{

/*
 * @brief Enumeration of all available types of spaceships
 */
enum class SpaceshipType : uint8_t
{
    x_wing,
    y_wing,
    millenium_falcon
};

/*
 * @file Spaceship.hpp
 * @brief The Spaceship class represents a single Spaceship object
 *
 * @details The class handles selecting the right texture, movement and shooting functionallity.
 *
 * @author Felix Hommel
 * @date Nov 23, 2024
 */
class Spaceship : public GameObject
{
public:
    /** Constructor / Destructor */
    Spaceship(SpaceshipType type, ColorType color, const WindowInfo* windowInfo);
    ~Spaceship() override = default;

    Spaceship(const Spaceship&) = default;
    Spaceship(Spaceship&&) = delete;
    Spaceship& operator=(const Spaceship&) = default;
    Spaceship& operator=(Spaceship&&) = delete;

    /** Public member functions */
    void update(float dt) override;

    [[nodiscard]] std::vector<Projectile>& getProjectiles() { return m_shooting->getProjectiles(); }

    [[nodiscard]] static std::string spaceshipTypeToString(SpaceshipType type);

private:
    /** Default values */
    static constexpr float movementSpeedFactor{ 350.f };
    glm::vec2 defaultPosition;
    glm::vec2 defaultSize;

    /** Member variables */
    SpaceshipType m_type;
    float m_speed;
    float m_shotPower;

    std::shared_ptr<MovementComponent> m_movement;
    std::shared_ptr<ShootingComponent> m_shooting;
    bool m_shot;

    /** Member functions */
    void readDataFromFile();
    void loadTexture(ColorType color);
};

} // namespace sfa

#endif //! GAME_OBJECTS_SPACESHIP_SPACESHIP_HPP

