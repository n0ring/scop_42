#include "KeyboardManager.hpp"

void KeyboardManager::update(ModelState &modelState)
{
	for (auto [key, state] : keysState)
	{
		if (state == GLFW_PRESS || state == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_UP:
				modelState.moveUp();
				break;
			case GLFW_KEY_DOWN:
				modelState.moveDown();
				break;
			case GLFW_KEY_LEFT:
				modelState.moveLeft();
				break;
			case GLFW_KEY_RIGHT:
				modelState.moveRight();
				break;
			case GLFW_KEY_MINUS:
				modelState.decreaseScale();
				break;
			case GLFW_KEY_EQUAL:
				if (keysState[GLFW_KEY_LEFT_SHIFT] > 0)
					modelState.increaseScale();
				break;
			case GLFW_KEY_F:
				modelState.fill_model = true;
				break;
			case GLFW_KEY_U:
				modelState.fill_model = false;
				break;
			default:
				break;
			}
		}
	}
}

void KeyboardManager::setKeyState(int key, int action)
{
	if (keysState.count(key))
		keysState[key] = action;
}
