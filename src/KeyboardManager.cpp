#include "KeyboardManager.hpp"

void KeyboardManager::update(ModelState &modelState)
{
	for (int keyNum = 0; keyNum < keysState.size(); keyNum++)
	{
		if (keysState[keyNum])
		{
			switch (keyNum + KEY_SHIFT)
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
			default:
				break;
			}
		}
	}
}

void KeyboardManager::setKeyState(int key, int action)
{
	if (key >= GLFW_KEY_RIGHT && key <= GLFW_KEY_UP)
		keysState[key - KEY_SHIFT] = action;
}
