#ifndef TEXT_COMPONENT_H_
#define TEXT_COMPONENT_H_

#include <string>
#include <functional>
#include <utility>

#include <glm/glm.hpp>

struct TextComponent {
	std::string text;
        glm::vec4 color;

	TextComponent(std::string text = "", glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		: text(std::move(text)), color(color) {}
};

#endif // TEXT_COMPONENT_H_
