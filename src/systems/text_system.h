#ifndef UI_TEXT_SYSTEM_H_
#define UI_TEXT_SYSTEM_H_

#include <tigr.h>

#include "../ecs/ecs.h"
#include "../components/transform_component.h"
#include "../components/text_component.h"

class TextSystem : public System {
public:
	TextSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<TextComponent>();
	}

	void Update(Tigr* window)
	{
		for (auto& entity : GetSystemEntities()) {
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& ui = entity.GetComponent<TextComponent>();

			tigrPrint(
				window,
				tfont,
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				ui.color,
				ui.text.c_str());
		}
	}
};

#endif // UI_TEXT_SYSTEM_H_
