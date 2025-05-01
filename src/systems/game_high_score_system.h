#ifndef HIGH_SCORE_SYSTEM_H_
#define HIGH_SCORE_SYSTEM_H_

#include <fstream>
#include <iomanip>
#include <sstream>

#include "../ecs/ecs.h"
#include "../components/game_high_score_component.h"
#include "../components/text_component.h"

/*
 * Updates the high score if needed and renders it in main menu.
 */
class GameHighScoreSystem : public System {
public:
	GameHighScoreSystem()
	{
		RequireComponent<TextComponent>();
		RequireComponent<GameHighScoreComponent>();
	}

	void Update()
	{
		for (auto& entity : GetSystemEntities()) {
			auto& text = entity.GetComponent<TextComponent>();

			std::ostringstream oss;
			oss << "HI " << std::fixed << std::setprecision(0)
			    << std::setw(3) << std::setfill('0')
			    << high_score_;

			text.text = oss.str();
		}
	}

	void SetHighScoreIfGreater(float score)
	{
		if (score <= 0.0f || score <= high_score_)
			return;

		high_score_ = score;
		SaveToFile();
	}

	void LoadHighScore()
	{
		LoadFromFile();
	}

private:
	void LoadFromFile()
	{
		float value = 0.0f;
		std::ifstream in("highscore.txt");
		if (in.is_open()) {
			in >> value;
			high_score_ = value;
		}
	}

	void SaveToFile()
	{
		std::ofstream out("highscore.txt", std::ios::trunc);
		if (out.is_open())
			out << high_score_;
	}

        float high_score_{0.0f};
};

#endif // HIGH_SCORE_SYSTEM_H_
