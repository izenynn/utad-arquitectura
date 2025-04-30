#ifndef INPUT_SYSTEM_H_
#define INPUT_SYSTEM_H_

#include <tigr.h>

#include "../ecs/ecs.h"
#include "../components/input_component.h"

class InputSystem : public System {
public:
	InputSystem()
	{
		RequireComponent<InputComponent>();
	}

	void Update(Tigr* window)
	{
		static std::unordered_map<int, bool> previous_key_states;

		constexpr int special_keys[] = {TK_LEFT, TK_RIGHT, TK_UP, TK_DOWN, TK_SPACE, TK_ESCAPE};

		for (const auto& entity : GetSystemEntities()) {
			auto& input = entity.GetComponent<InputComponent>();

			input.keys_pressed.clear();
			input.keys_held.clear();
			input.keys_released.clear();

			auto handle_key = [&](int key) {
				bool down_now = tigrKeyDown(window, key);
				bool held_now = tigrKeyHeld(window, key);
				bool was_down = previous_key_states[key];

				if (down_now && !was_down)
					input.keys_pressed.insert(key);
				if (held_now)
					input.keys_held.insert(key);
				if (!held_now && was_down)
					input.keys_released.insert(key);

				previous_key_states[key] = held_now;
			};

			for (int key = 'A'; key <= 'Z'; key++) {
				handle_key(key);
			}
			for (int key : special_keys) {
				handle_key(key);
			}
		}
	}
};

#endif // INPUT_SYSTEM_H_
