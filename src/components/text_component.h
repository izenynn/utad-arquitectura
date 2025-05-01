#ifndef TEXT_COMPONENT_H_
#define TEXT_COMPONENT_H_

#include <string>
#include <functional>
#include <utility>

#include <tigr.h>

struct TextComponent {
	std::string text;
        TPixel color;

	TextComponent(std::string text = "", TPixel color = tigrRGBA(0xff, 0xff, 0xff, 0xff))
		: text(std::move(text)), color(color) {}
};

#endif // TEXT_COMPONENT_H_

