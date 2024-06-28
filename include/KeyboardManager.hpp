#pragma once

#include <vector>
#include "ObjectRenderer.hpp"

#define KEY_SHIFT GLFW_KEY_RIGHT

class KeyboardManager
{
public:
	KeyboardManager() : keysState(4) {}
	void update(ModelState &modelState);
	void setKeyState(int key, int action);
private:
	// int keysArr[4] = {GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_UP};
	std::vector<bool> keysState;
};


// #define GLFW_KEY_RIGHT              262
// #define GLFW_KEY_LEFT               263
// #define GLFW_KEY_DOWN               264
// #define GLFW_KEY_UP                 265