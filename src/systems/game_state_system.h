#ifndef GAME_STATE_SYSTEM_H_
#define GAME_STATE_SYSTEM_H_

#include "../ecs/ecs.h"

#include "../components/sprite_component.h"
#include "../components/input_component.h"
#include "../components/game_state_component.h"

#include "../systems/game_start_system.h"
#include "../systems/game_reset_system.h"

class GameStateSystem : public System {
public:
	GameStateSystem()
	{
		RequireComponent<InputComponent>();
		RequireComponent<GameStateComponent>();
	}

	void Update(Registry& registry)
	{
		// Should only be one entity, check
		if (GetSystemEntities().size() != 1) {
			Logger::Error("Game State System: Expected one game_manager entity, found: " + std::to_string(GetSystemEntities().size()));
			return;
		}

		auto& gm = GetSystemEntities()[0];

		auto& input = gm.GetComponent<InputComponent>();
		auto& game_state = gm.GetComponent<GameStateComponent>();

		if (game_state.state == GameState::Init) {
			if (registry.HasSystem<GameResetSystem>()) {
				registry.GetSystem<GameResetSystem>().Run(registry);
			} else {
				Logger::Error("Game State System: GameResetSystem not found");
			}
			game_state.state = GameState::Menu;
		}

		if (game_state.state == GameState::Menu && input.keys_held.count(TK_SPACE)) {
			if (registry.HasSystem<GameStartSystem>()) {
				registry.GetSystem<GameStartSystem>().Run(registry);
				game_state.state = GameState::Playing;
			} else {
				Logger::Error("Game State System: GameStartSystem not found");
			}
		}
	}
};

#endif // GAME_STATE_SYSTEM_H_
