#ifndef CORE_END_STATE_HPP
#define CORE_END_STATE_HPP

#include "internal/Button.hpp"
#include "Score.hpp"
#include "State.hpp"

#include <array>

/*
 * @file EndState.hpp
 * @brief This class represents the End Screen
 *
 * @details It handles connecting to the Database and displays topX amount of scores to the player.
 *
 * @author Felix Hommel
 * @date Nov 18, 2024
*/
class EndState : public State
{
	public:
		/** constructor / Destructor */
		EndState(const Texture2D& background, const WindowInfo* windowInfo, const Mouse* mouse, const Score& currentScore);
		~EndState() override;

		/** Delete move constructor / assignment operator and leave the copy constructor / assignment operator at default */
		EndState(const EndState&) = default;
		EndState(EndState&&) = delete;
		EndState& operator=(const EndState&) = default;
		EndState& operator=(EndState&&) = delete;

		/** Public member functions */
		void update(float dt) override;
		void render(SpriteRenderer& renderer, TextRenderer& textRenderer) override;

		[[nodiscard]] bool getBackButtonPressed() const { return m_backButtonPressed; }

        /** Predefined SQL Statements */
		static constexpr auto checkIfTableExists {R"(
            CREATE TABLE IF NOT EXISTS scores 
            (id INTEGER PRIMARY KEY AUTOINCREMENT,
            score INTEGER,
            wave INTEGER)
        )"};
        /** SELECT RAISE(IGNORE) will make it so that the insertion fails silently if the score does not qualify */
        static constexpr auto maxScoreboardLengthTrigger {R"(
            CREATE TRIGGER IF NOT EXISTS maxLength 
            BEFORE INSERT ON scores 
            WHEN (SELECT COUNT(*) FROM scores WHERE score >= NEW.score) >= 100 
            BEGIN 
                SELECT RAISE(IGNORE); 
            END;
        )"};
		static constexpr auto insertNewScoreQuery {R"(
            INSERT INTO scores (score, wave) 
            VALUES (?, ?)
        )"};
		static constexpr auto getTopXQuery {R"(
            SELECT * 
            FROM scores 
            ORDER BY score DESC, wave DESC 
            LIMIT ?
        )"};

	private:
		/** Constants */
		static constexpr int topX {10};

		/** Member variables */
		Button* m_backButton;
		bool m_backButtonPressed;

        Texture2D m_spaceshipTexture;

		std::array<Score, topX + 1> m_scoreboard; //m_scoreboard.at(0) will always be the current score

		/** Private member functions */
		void saveNewScore();
		void getScores();
		void renderScoreboard(TextRenderer& textRenderer);
};

#endif //!CORE_END_STATE_HPP
