#include "core/WindowInfo.hpp"
#include "gameObjects/EnemyWave.hpp"
#include "gameObjects/EnemyWaveGenerator.hpp"
#include "gameObjects/MovementComponent.hpp"
#include "gameObjects/ShootingComponent.hpp"
#include "internal/JSONReader.hpp"
#include "MockTexture.hpp"
#include "core/EndState.hpp"

#include "SQLiteCpp/SQLiteCpp.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

/** JSON TEST */
TEST(JSON, ReadSingleValue)
{
	JSONReader r;
	r.open("resources/config/x_wing.json");

	auto val {r.get<unsigned int>("lives")};
	ASSERT_TRUE(val.has_value());
	ASSERT_EQ(3, val.value());
}

TEST(JSON, ReadAllValues)
{
	JSONReader r;
	r.open("resources/config/x_wing.json");

	auto val1 {r.get<unsigned int>("lives")};
	ASSERT_TRUE(val1.has_value());
	ASSERT_EQ(3, val1.value());

	auto val2 {r.get<unsigned int>("speed")};
	ASSERT_TRUE(val2.has_value());
	ASSERT_EQ(3, val2.value());

	auto val3 {r.get<unsigned int>("shotPower")};
	ASSERT_TRUE(val3.has_value());
	ASSERT_EQ(10, val3.value());
}

TEST(JSON, ReadNonExistingValue)
{
	JSONReader r;
	r.open("resources/config/x_wing.json");

	auto val1 {r.get<unsigned int>("live")};
	ASSERT_FALSE(val1.has_value());
}

TEST(JSON, AttemptOpenNonExistingFile)
{
	JSONReader r;
	ASSERT_FALSE(r.open("resources/config/z_wing.json"));
}

TEST(JSON, AttemptOpenInvalidFile)
{
	JSONReader r;
	ASSERT_FALSE(r.open("test/resources/invalid.json"));
}

/** SPACESHIP MOVEMENT TEST */

TEST(SPACESHIP_MOVEMENT, MOVE_LEFT_FREE)
{
	glm::vec2 windowSize(500, 500);
	glm::vec2 position(300, 300);
	glm::vec2 size(100, 50);
	float moveBy {-50.f};
	float expected {position.x + moveBy};

	MovementComponent m(&position, size, windowSize.x, windowSize.y);
	m.moveX(moveBy);

	ASSERT_FLOAT_EQ(expected, position.x);
}

TEST(SPACESHIP_MOVEMENT, MOVE_LEFT_EDGE)
{
	glm::vec2 windowSize(500, 500);
	glm::vec2 position(30, 300);
	glm::vec2 size(100, 50);
	float moveBy {-50.f};
	float expected {0};

	MovementComponent m(&position, size, windowSize.x, windowSize.y);
	m.moveX(moveBy);

	ASSERT_FLOAT_EQ(expected, position.x);
}

TEST(SPACESHIP_MOVEMENT, MOVE_RIGHT_FREE)
{
	glm::vec2 windowSize(500, 500);
	glm::vec2 position(300, 300);
	glm::vec2 size(100, 50);
	float moveBy {50.f};
	float expected {position.x + moveBy};

	MovementComponent m(&position, size, windowSize.x, windowSize.y);
	m.moveX(moveBy);

	ASSERT_FLOAT_EQ(expected, position.x);
}

TEST(SPACESHIP_MOVEMENT, MOVE_RIGHT_EDGE)
{
	glm::vec2 windowSize(500, 500);
	glm::vec2 position(390, 300);
	glm::vec2 size(100, 50);
	float moveBy {50.f};
	float expected {windowSize.x - size.x};

	MovementComponent m(&position, size, windowSize.x, windowSize.y);
	m.moveX(moveBy);

	ASSERT_FLOAT_EQ(expected, position.x);
}

TEST(SPACESHIP_MOVEMENT, MOVE_UP_FREE)
{
	glm::vec2 windowSize(500, 500);
	glm::vec2 position(300, 300);
	glm::vec2 size(100, 50);
	float moveBy {-50.f};
	float expected {position.y + moveBy};

	MovementComponent m(&position, size, windowSize.x, windowSize.y);
	m.moveY(moveBy);

	ASSERT_FLOAT_EQ(expected, position.y);
}

TEST(SPACESHIP_MOVEMENT, MOVE_UP_EDGE)
{
	glm::vec2 windowSize(500, 500);
	glm::vec2 position(300, 40);
	glm::vec2 size(100, 50);
	float moveBy {-50.f};
	float expected {0};

	MovementComponent m(&position, size, windowSize.x, windowSize.y);
	m.moveY(moveBy);

	ASSERT_FLOAT_EQ(expected, position.y);
}

TEST(SPACESHIP_MOVEMENT, MOVE_DOWN_FREE)
{
	glm::vec2 windowSize(500, 500);
	glm::vec2 position(300, 300);
	glm::vec2 size(100, 50);
	float moveBy {50.f};
	float expected {position.y + moveBy};

	MovementComponent m(&position, size, windowSize.x, windowSize.y);
	m.moveY(moveBy);

	ASSERT_FLOAT_EQ(expected, position.y);
}

TEST(SPACESHIP_MOVEMENT, MOVE_DOWN_EDGE)
{
	glm::vec2 windowSize(500, 500);
	glm::vec2 position(300, 430);
	glm::vec2 size(100, 50);
	float moveBy {50.f};
	float expected {windowSize.y - size.y};

	MovementComponent m(&position, size, windowSize.x, windowSize.y);
	m.moveY(moveBy);

	ASSERT_FLOAT_EQ(expected, position.y);
}

/** WAVE TESTS */
TEST(WAVE_GENERATION, SINGLE_TEST)
{
	WindowInfo w {500, 500, 60, "test"};

	MockTexture2D mockTexture;
	EXPECT_CALL(mockTexture, generate(256, 256, testing::_)).Times(1);

	unsigned char* dummyData = new unsigned char[10];
	mockTexture.generate(256, 256, dummyData);

	EnemyWave wave(&w, mockTexture, mockTexture);
	bool valid {false};
	wave.checkForBigEnoughHole(valid);

	EXPECT_TRUE(valid);

	delete[] dummyData;
}

TEST(WAVE_GENERATION, MULTIPLE_TEST)
{
    const size_t testCases{ 25 };
    for(size_t i{ 0 }; i < testCases; ++i)
    {
        WindowInfo w {500, 500, 60, "test"};
        unsigned char* dummyData = new unsigned char[10];
        const int texW{ 10 };
        const int texH{ 10 };

        MockTexture2D mockTexture;
        EXPECT_CALL(mockTexture, generate(texW, texH, testing::_)).Times(1);
        mockTexture.generate(texW, texH, dummyData);

        EnemyWave wave(&w, mockTexture, mockTexture);
        bool valid {false};
        wave.checkForBigEnoughHole(valid);

        EXPECT_TRUE(valid);

        delete[] dummyData;
    }
}

/* 
* For simplicity Collision will be tested with two meteorite objects. 
* This does not affect the results, because meteorite as Spaceship inherits from GameObject and are therfore the same thing for collision testing.
*/
TEST(COLLISION_AABB, COLLISION_FALSE)
{
    WindowInfo w {500, 500, 60, "test"};
    unsigned char* dummyData = new unsigned char[10];
    const int texW{ 10 };
    const int texH{ 10 };

    MockTexture2D mockTexture;
    EXPECT_CALL(mockTexture, generate(texW, texH, testing::_)).Times(1);
    mockTexture.generate(texW, texH, dummyData);

    glm::vec2 pos_m1(30.f, 30.f);
    Meteorite m1(mockTexture, MeteoriteType::SMALL, pos_m1, &w);

    glm::vec2 pos_m2(pos_m1.x + m1.getSize().x + 1, 30.f);
    Meteorite m2(mockTexture, MeteoriteType::SMALL, pos_m2, &w);

    EXPECT_FALSE(GameObject::checkCollisionAABB(m1, m2));
}

TEST(COLLISION_AABB, COLLISION_TRUE)
{

    WindowInfo w {500, 500, 60, "test"};
    unsigned char* dummyData = new unsigned char[10];
    const int texW{ 10 };
    const int texH{ 10 };

    MockTexture2D mockTexture;
    EXPECT_CALL(mockTexture, generate(texW, texH, testing::_)).Times(1);
    mockTexture.generate(texW, texH, dummyData);

    glm::vec2 pos_m1(30.f, 30.f);
    Meteorite m1(mockTexture, MeteoriteType::SMALL, pos_m1, &w);

    glm::vec2 pos_m2(pos_m1.x + m1.getSize().x, 30.f);
    Meteorite m2(mockTexture, MeteoriteType::SMALL, pos_m2, &w);

    EXPECT_TRUE(GameObject::checkCollisionAABB(m1, m2));
}

/** Shooting tests */
TEST(SHOOTING, SINGLE_SHOT)
{
    WindowInfo w {500, 500, 60, "test"};
    unsigned char* dummyData = new unsigned char[10];
    constexpr int texW{ 10 };
    constexpr int texH{ 10 };

    MockTexture2D mockTexture;
    EXPECT_CALL(mockTexture, generate(texW, texH, testing::_)).Times(1);
    mockTexture.generate(texW, texH, dummyData);

    constexpr glm::vec2 pos(100.f, 100.f);
    constexpr int damage{ 1 };
    bool didShoot{ false };

    ShootingComponent shooting(mockTexture, &pos, damage, &w);

    size_t prev{ shooting.getProjectiles().size() };

    shooting.shoot(didShoot);

    EXPECT_EQ(shooting.getProjectiles().size(), prev + 1);
}

TEST(SHOOTING, MULTI_SHOT)
{
    WindowInfo w {500, 500, 60, "test"};
    unsigned char* dummyData{ new unsigned char[10] };
    constexpr int texW{ 10 };
    constexpr int texH{ 10 };

    MockTexture2D mockTexture;
    EXPECT_CALL(mockTexture, generate(texW, texH, testing::_)).Times(1);
    mockTexture.generate(texW, texH, dummyData);

    constexpr glm::vec2 pos(100.f, 100.f);
    constexpr int damage{ 1 };
    constexpr int shotsFired{ 25 };
    constexpr float timeProgress{ 0.3f };
    bool didShoot{ false };

    ShootingComponent shooting(mockTexture, &pos, damage, &w);

    size_t prev{ shooting.getProjectiles().size() };

    for(int i{ 0 }; i < shotsFired; ++i)
    {
        shooting.update(timeProgress);
        shooting.shoot(didShoot);
    }

    EXPECT_EQ(shooting.getProjectiles().size(), prev + shotsFired);
}

TEST(SHOOTING, COOLDOWN)
{
    WindowInfo w {500, 500, 60, "test"};
    unsigned char* dummyData = new unsigned char[10];
    constexpr int texW{ 10 };
    constexpr int texH{ 10 };

    MockTexture2D mockTexture;
    EXPECT_CALL(mockTexture, generate(texW, texH, testing::_)).Times(1);
    mockTexture.generate(texW, texH, dummyData);

    constexpr glm::vec2 pos(100.f, 100.f);
    constexpr int damage{ 1 };
    bool didShoot{ false };

    ShootingComponent shooting(mockTexture, &pos, damage, &w);

    shooting.shoot(didShoot);
    EXPECT_TRUE(didShoot);

    shooting.shoot(didShoot);
    EXPECT_FALSE(didShoot);

    shooting.shoot(didShoot);
    EXPECT_FALSE(didShoot);

    shooting.update(0.3f);
    shooting.shoot(didShoot);
    EXPECT_TRUE(didShoot);
}

/** Highscore tests */
TEST(HIGHSCORES, SCORE_INCREASE)
{
    WindowInfo w {500, 500, 60, "test"};
    unsigned char* dummyData = new unsigned char[10];
    constexpr int texW{ 10 };
    constexpr int texH{ 10 };

    MockTexture2D mockTexture;
    EXPECT_CALL(mockTexture, generate(texW, texH, testing::_)).Times(1);
    mockTexture.generate(texW, texH, dummyData);

    EnemyWaveGenerator gen(mockTexture, mockTexture, &w);

    size_t enemiesInWave{ gen.getEnemyWaves()[0].getEnemies().size() };
    gen.getEnemyWaves()[0].getEnemies().clear();

    gen.update(0.025f);

    unsigned int score{ 0 };
    while(!gen.getPoints().empty())
    {
        score += gen.getPoints().back();
        gen.getPoints().pop();
    }

    EXPECT_EQ(enemiesInWave * 10, score);
}

/*
 * Test if the DB inserted correctly. exec() returns the number of rows that it modified, therefore
 * it expectedly should return 1
*/
TEST(HIGHSCORES, WRITE_TO_DB_IF_SCORE_IS_HIGHER)
{
    int wave{ 45 };
    int score{ 12497561 };
    int modified{ 0 };

    try
    {
        SQLite::Database db("test/resources/test.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

        SQLite::Transaction insert(db);

        db.exec(EndState::checkIfTableExists);

        SQLite::Statement ensureMaxLength(db, EndState::maxScoreboardLengthTrigger);

        SQLite::Statement insertQuery(db, EndState::insertNewScoreQuery);
        insertQuery.bind(1, score);
        insertQuery.bind(2, wave);

        modified = insertQuery.exec();

        insert.commit();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    EXPECT_EQ(modified, 1);
}

/** Difficulty increase test */
TEST(DIFFICULTY, INCREASE)
{
    WindowInfo w {500, 500, 60, "test"};
    unsigned char* dummyData = new unsigned char[10];
    constexpr int texW{ 10 };
    constexpr int texH{ 10 };

    MockTexture2D mockTexture;
    EXPECT_CALL(mockTexture, generate(texW, texH, testing::_)).Times(1);
    mockTexture.generate(texW, texH, dummyData);

    EnemyWaveGenerator gen(mockTexture, mockTexture, &w);
    EXPECT_FLOAT_EQ(1.f, gen.getCurrentEnemySpeed());

    for(int i{ 0 }; i < 4; ++i)
        gen.genWave();

    EXPECT_FLOAT_EQ(1.25f, gen.getCurrentEnemySpeed());


    for(int i{ 0 }; i < 3; ++i)
    {
        for(int j{ 0 }; j < 5; ++j)
            gen.genWave();

        EXPECT_FLOAT_EQ(1.5f + (i * 0.25f), gen.getCurrentEnemySpeed());
    }
}
