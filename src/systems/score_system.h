#ifndef SCORE_SYSTEM_H_
#define SCORE_SYSTEM_H_

#include <iomanip>
#include <sstream>

#include "../ecs/ecs.h"
#include "../components/text_component.h"
#include "../components/score_component.h"

class ScoreSystem : public System {
public:
	ScoreSystem()
	{
		RequireComponent<TextComponent>();
		RequireComponent<ScoreComponent>();
	}

	void Update(float delta_time)
	{
		for (auto& entity : GetSystemEntities()) {
			auto& text = entity.GetComponent<TextComponent>();
			auto& score = entity.GetComponent<ScoreComponent>();

			score.time_survived += delta_time;

			std::ostringstream oss;
			oss << std::fixed << std::setprecision(0)
			    << std::setw(3) << std::setfill('0')
			    << score.time_survived;

			text.text = oss.str();
		}
	}
};

#endif // SCORE_SYSTEM_H_

