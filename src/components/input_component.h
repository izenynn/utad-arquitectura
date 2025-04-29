#ifndef INPUT_COMPONENT_H_
#define INPUT_COMPONENT_H_

#include <set>

struct InputComponent {
	std::set<int> keys_pressed;
	std::set<int> keys_held;
	std::set<int> keys_released;
};

#endif // INPUT_COMPONENT_H_

