#pragma once 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



#define MOVE_STEP 1.0f
#define SCALE_STEP 0.01f


struct ModelState
{
	float rotation_x = 0.001f;
	float rotation_y = 0.001f;
	glm::vec3 scale;
	bool fill_model = false;

	ModelState() : scale(1.0f, 1.0f, 1.0f) {}

	void moveX(float val)
	{
		rotation_x += val;
		if (rotation_x >= 360)
			rotation_x = 0;
		else if (rotation_x <= 0)
			rotation_x = 360;
	}

	void moveY(float val)
	{
		rotation_y += val;
		if (rotation_y >= 360)
			rotation_y = 0;
		else if (rotation_y <= 0)
			rotation_y = 360;
	}

	void moveUp()
	{
		moveY(MOVE_STEP);
	}
	void moveDown()
	{
		moveY(-MOVE_STEP);
	}
	void moveLeft()
	{
		moveX(-MOVE_STEP);
	}
	void moveRight()
	{
		moveX(MOVE_STEP);
	}

	void increaseScale()
	{
		scale.z += SCALE_STEP;
		scale.x += SCALE_STEP;
		scale.y += SCALE_STEP;
		if (scale.x > 2.5f)
		{
			scale.z = 2.5f;
			scale.x = 2.5f;
			scale.y = 2.5f;
		}
	}

	void decreaseScale()
	{
		scale.z -= SCALE_STEP;
		scale.x -= SCALE_STEP;
		scale.y -= SCALE_STEP;
		if (scale.z < 0)
		{
			scale.z = 0;
			scale.x = 0;
			scale.y = 0;
		}
	}
};