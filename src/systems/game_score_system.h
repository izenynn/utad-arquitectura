#ifndef GAME_SCORE_SYSTEM_H_
#define GAME_SCORE_SYSTEM_H_

#include <iomanip>
#include <sstream>

#include "../ecs/ecs.h"
#include "../components/text_component.h"
#include "../components/game_score_component.h"

class GameScoreSystem : public System {
public:
	GameScoreSystem()
	{
		RequireComponent<TextComponent>();
		RequireComponent<GameScoreComponent>();
	}

	void Update(float delta_time)
	{
		if (!active_)
			return;

		for (auto& entity : GetSystemEntities()) {
			auto& text = entity.GetComponent<TextComponent>();

			score_ += delta_time;

			std::ostringstream oss;
			oss << std::fixed << std::setprecision(0)
			    << std::setw(3) << std::setfill('0')
			    << score_;

			text.text = oss.str();
		}
	}

	void Reset()
	{
		score_ = 0.0f;
		active_ = true;
	}

	void Stop()
	{
		active_ = false;
	}

	float GetScore() const
	{
		return score_;
	}

private:
	bool active_{false};
	float score_{0.0f};
};

#endif // GAME_SCORE_SYSTEM_H_

