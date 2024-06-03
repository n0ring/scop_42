#pragma once

#include "imgui.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void onUpdate(float) {}
		virtual void onRender() {}
		virtual void onImGuiRender() {}


	private:
	};
};