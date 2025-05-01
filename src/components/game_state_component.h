#ifndef GAME_STATE_COMPONENT_H_
#define GAME_STATE_COMPONENT_H_

enum class GameState {
	Init,
	Menu,
	Playing
};

struct GameStateComponent {
	GameState state;

	GameStateComponent(GameState state = GameState::Init)
		: state(state) {}
};

#endif // GAME_STATE_COMPONENT_H_
