#pragma once

#include <unordered_map>
#include "ObjectRenderer.hpp"

#define KEY_SHIFT GLFW_KEY_RIGHT

class KeyboardManager
{
public:
	KeyboardManager() {}
	void update(ModelState &modelState);
	void setKeyState(int key, int action);
private:
	// int keysArr[4] = {GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_UP};
	std::unordered_map<int, int> keysState = 	{
		{GLFW_KEY_RIGHT, 0},
		{GLFW_KEY_LEFT, 0},
		{GLFW_KEY_DOWN, 0},
		{GLFW_KEY_UP, 0},
		{GLFW_KEY_EQUAL, 0},
		{GLFW_KEY_LEFT_SHIFT, 0},
		{GLFW_KEY_MINUS, 0},
		{GLFW_KEY_F, 0},
		{GLFW_KEY_U, 0},
		{GLFW_KEY_W, 0},
		{GLFW_KEY_S, 0},
		{GLFW_KEY_D, 0},
		{GLFW_KEY_A, 0},
	};
};


// #define GLFW_KEY_RIGHT              262
// #define GLFW_KEY_LEFT               263
// #define GLFW_KEY_DOWN               264
// #define GLFW_KEY_UP                 265