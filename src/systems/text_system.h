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
			auto& text = entity.GetComponent<TextComponent>();

			// Get TPixel color from glm::vec4
			TPixel color = tigrRGBA(
				static_cast<unsigned char>(text.color.x * 255),
				static_cast<unsigned char>(text.color.y * 255),
				static_cast<unsigned char>(text.color.z * 255),
				static_cast<unsigned char>(text.color.w * 255));

			tigrPrint(
				window,
				tfont,
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				color,
				text.text.c_str());
		}
	}
};

#endif // UI_TEXT_SYSTEM_H_
