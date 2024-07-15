#include "KeyboardManager.hpp"

void KeyboardManager::update()
{
	for (auto [key, state] : keysState)
	{
		if (state == GLFW_PRESS || state == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_UP:
				m_observers[m_activeObject]->getModelState().moveUp();
				break;
			case GLFW_KEY_DOWN:
				m_observers[m_activeObject]->getModelState().moveDown();
				break;
			case GLFW_KEY_LEFT:
				m_observers[m_activeObject]->getModelState().moveLeft();
				break;
			case GLFW_KEY_RIGHT:
				m_observers[m_activeObject]->getModelState().moveRight();
				break;
			case GLFW_KEY_MINUS:
				m_observers[m_activeObject]->getModelState().decreaseScale();
				break;
			case GLFW_KEY_EQUAL:
				if (keysState[GLFW_KEY_LEFT_SHIFT] > 0)
					m_observers[m_activeObject]->getModelState().increaseScale();
				break;
			case GLFW_KEY_F:
				m_observers[m_activeObject]->getModelState().fill_model = true;
				break;
			case GLFW_KEY_U:
				m_observers[m_activeObject]->getModelState().fill_model = false;
				break;
			case GLFW_KEY_W:
				m_observers[m_activeObject]->getModelState().moveModelUp();
				break;
			case GLFW_KEY_S:
				m_observers[m_activeObject]->getModelState().moveModelDown();
				break;
			case GLFW_KEY_D:
				m_observers[m_activeObject]->getModelState().moveModelRight();
				break;
			case GLFW_KEY_A:
				m_observers[m_activeObject]->getModelState().moveModelLeft();
				break;
			case GLFW_KEY_E:
				m_observers[m_activeObject]->getModelState().moveModelFar();
				break;
			case GLFW_KEY_Q:
				m_observers[m_activeObject]->getModelState().moveModelClose();
				break;
			case GLFW_KEY_T:
				m_observers[m_activeObject]->getModelState().changeToTexture();
				break;
			case GLFW_KEY_C:
				m_observers[m_activeObject]->getModelState().changeToColor();
				break;
			case GLFW_KEY_1:
				m_activeObject = 0;
				break;
			case GLFW_KEY_2:
				m_activeObject = 1;
				break;
			default:
				break;
			}
		}
	}
}

void KeyboardManager::spellCheck()
{
		for (auto [key, state] : keysState)
	{
		if (state == GLFW_PRESS || state == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_U:
				if (kLumos[m_lumosIdx] == 'u')
					m_lumosIdx++;
				break;
			case GLFW_KEY_S:
				if (kLumos[m_lumosIdx] == 's')
					m_lumosIdx++;
				break;
			case GLFW_KEY_L:
				if (kLumos[m_lumosIdx] == 'l')
					m_lumosIdx++;
				break;
			case GLFW_KEY_N:
				if (kNox[m_noxIdx] == 'n')
					m_noxIdx++;
				break;
			case GLFW_KEY_M:
				if (kLumos[m_lumosIdx] == 'm')
					m_lumosIdx++;
				break;
			case GLFW_KEY_O:
				if (kLumos[m_lumosIdx] == 'o')
					m_lumosIdx++;
				if (kNox[m_noxIdx] == 'o')
					m_noxIdx++;
				break;
			case GLFW_KEY_X:
				if (kNox[m_noxIdx] == 'x')
					m_noxIdx++;
				break;
			default:
				break;
			}
		}
	}
	if (m_lumosIdx == kLumos.size())
	{
		for (auto& obj : m_observers)	
			obj->getModelState().lumos();
		m_lumosIdx = 0;
	}
	if (m_noxIdx == kNox.size())
	{
		for (auto& obj : m_observers)	
			obj->getModelState().nox();
		m_noxIdx = 0;
	}
}


void KeyboardManager::setKeyState(int key, int action)
{
	if (keysState.count(key))
		keysState[key] = action;
}

void KeyboardManager::addObserver(ObjectRenderer *p_object)
{
	m_observers.push_back(p_object);
}
