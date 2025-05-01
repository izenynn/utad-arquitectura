#ifndef TAG_COMPONENT_H_
#define TAG_COMPONENT_H_

#include <string>

struct TagComponent {
	std::string tag;

	explicit TagComponent(std::string tag = "")
		: tag(std::move(tag)) {}
};

#endif // TAG_COMPONENT_H_
