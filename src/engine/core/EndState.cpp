#include "EndState.hpp"

#include "internal/ResourceManager.hpp"

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "SQLiteCpp/Transaction.h"

#include <iostream>

EndState::EndState(const Texture2D& background, const WindowInfo* windowInfo, const Mouse* mouse, const Score& currentScore)
	: State(background, windowInfo, mouse, StateName::End)
	, m_backButton(nullptr)
	, m_backButtonPressed(false)
    , m_spaceshipTexture(ResourceManager::getTexture("spaceship"))
    , m_scoreboard()
{
	m_backButton = new Button(glm::vec2(m_windowInfo->width * 0.025, m_windowInfo->height * 0.85),
		glm::vec2(m_windowInfo->width * 0.2, m_windowInfo->height * 0.1), glm::vec3(158.f / 255.f, 80.f / 255.f, 250.f / 255.f),
		glm::vec3(100.f / 255.f, 33.f / 255.f, 177.f / 255.f), glm::vec3(81.f / 255.f, 8.f / 255.f, 166.f / 255.f), "Back",
		ResourceManager::getShader("button"));
	m_backButton->setOnClick([&backButtonPressed = m_backButtonPressed]() { backButtonPressed = true; });

	m_scoreboard.at(0) = currentScore;
    saveNewScore();
    getScores();
}

EndState::~EndState()
{
	delete m_backButton;
}

void EndState::update(float dt)
{
	m_backButton->update(m_mouse->posX, m_mouse->posY, m_mouse->isPressed);
}

void EndState::render(SpriteRenderer& renderer, TextRenderer& textRenderer)
{
	renderer.draw(m_background, glm::vec2(0.f, 0.f), glm::vec2(m_windowInfo->width, m_windowInfo->height));
	m_backButton->draw(&textRenderer);
	textRenderer.draw("GAME OVER", m_windowInfo->width / 2.f - m_windowInfo->width * 0.1f, m_windowInfo->height * 0.05f, 2.f);

	std::stringstream ss;
	ss << "You died on wave " << m_scoreboard.at(0).wave << " and got " << m_scoreboard.at(0).score << " points";
	textRenderer.draw(ss.str(), m_windowInfo->width / 2.f - m_windowInfo->width * 0.2f, m_windowInfo->height * 0.15f);
	renderScoreboard(textRenderer);

    renderer.draw(m_spaceshipTexture, glm::vec2(m_windowInfo->width * 0.05f, m_windowInfo->height * 0.4f), glm::vec2(m_windowInfo->width * 0.2f, m_windowInfo->height * 0.2f));
}

void EndState::saveNewScore()
{
    try
    {
        SQLite::Database db("resources/scores.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

        SQLite::Transaction insert(db);

        db.exec(checkIfTableExists);

        SQLite::Statement ensureMaxLength(db, maxScoreboardLengthTrigger);

        SQLite::Statement insertQuery(db, insertNewScoreQuery);
        insertQuery.bind(1, m_scoreboard.at(0).score);
        insertQuery.bind(2, m_scoreboard.at(0).wave);

        insertQuery.exec();

        insert.commit();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/*
 * @TODO
 * @brief Establish a connection to the Database and retrieve the topX scores out of it
*/
void EndState::getScores()
{
    try
    {
        SQLite::Database db("resources/scores.db3", SQLite::OPEN_READONLY);

        SQLite::Statement query(db, getTopXQuery);
        query.bind(1, topX);

        int idx{ 1 };
        while(query.executeStep())
        {
            m_scoreboard.at(idx).score = query.getColumn(1);
            m_scoreboard.at(idx).wave = query.getColumn(2);
            ++idx;
        }

    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void EndState::renderScoreboard(TextRenderer& textRenderer)
{
	/*
     * Maybe sorting is not neccessary, depends on db query, may be removed later. 
     * Also as long as there is no db to read from, this will print garbage memory values, because of non initizlization of the Score structs.
    */
	std::sort(m_scoreboard.begin() + 1, m_scoreboard.end(), [](Score a, Score b) { return a.score > b.score; });
    for(size_t i {1}; i < m_scoreboard.size(); ++i)
    {
        std::stringstream line;

        if(i < 10)
            line << "#" << i << "  - " << m_scoreboard.at(i).score << " Points on Wave " << m_scoreboard.at(i).wave;
        else
            line << "#" << i << " - " << m_scoreboard.at(i).score << " Points on Wave " << m_scoreboard.at(i).wave;

        textRenderer.draw(line.str(), m_windowInfo->width / 2.f - m_windowInfo->width * 0.2f,
                          (m_windowInfo->height * 0.225f) + (m_windowInfo->height * 0.05f * i));
    }
}
