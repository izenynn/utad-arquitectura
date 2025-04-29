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
		for (const auto& entity : GetSystemEntities()) {
			auto& input = entity.GetComponent<InputComponent>();
			input.keys_pressed.clear();
			input.keys_released.clear();

			for (int key = 'A'; key <= 'Z'; key++) {
				bool down = tigrKeyDown(window, key);
				bool held = tigrKeyHeld(window, key);

				if (down) input.keys_pressed.insert(key);
				if (held) input.keys_held.insert(key);
			}

			// Add arrow keys, space, etc.
			for (int key : {TK_LEFT, TK_RIGHT, TK_UP, TK_DOWN, TK_SPACE, TK_ESCAPE}) {
				bool down = tigrKeyDown(window, key);
				bool held = tigrKeyHeld(window, key);

				if (down) input.keys_pressed.insert(key);
				if (held) input.keys_held.insert(key);
			}
		}
	}
};

#endif // INPUT_SYSTEM_H_
