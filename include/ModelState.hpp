#pragma once 

// #include "glm/glm.hpp"
// #include "glm/gtc/matrix_transform.hpp"

#include "nrg_math.hpp"


#define MOVE_STEP 1.0f
#define SCALE_STEP 0.01f
#define MOVE_MODEL_STEP 0.1f
// #define MOVE_MODEL_STEP 0.1f

enum class RenderMode
{
	COLOR,
	TEXTURE
};


struct ModelState
{
	float rotation_x = 0.001f;
	float rotation_y = 0.001f;
	nrg::vec3 scale;
	bool fill_model = false;
	nrg::vec3 translation;
	RenderMode renderMode = RenderMode::COLOR;
	nrg::vec3 centerOffsetMin = {100000.0f};
	nrg::vec3 centerOffsetMax = {-100000.0f};
	bool hasNormals = false;
	nrg::vec3 lightPos;
	bool lightOn = false;

	ModelState() : scale(1.0f, 1.0f, 1.0f), 
		translation(nrg::vec3(0.5f)), 
		lightPos(0.0f, 10.0f, 3.0f)
		{}

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

	void moveModelUp()
	{
		// if (translation.y <= 3.0f)
			translation.y += MOVE_MODEL_STEP;
	}
	void moveModelDown()
	{
		// if (translation.y >= -3.0f)
			translation.y -= MOVE_MODEL_STEP;
	}

	void moveModelRight()
	{
		// if (translation.x <= 5.0f)
			translation.x += MOVE_MODEL_STEP;
	}
	void moveModelLeft()
	{
		// if (translation.x >= -5.0f)
			translation.x -= MOVE_MODEL_STEP;
	}

	void moveModelClose()
	{
		// if (translation.z >= -6.0f)
			translation.z -= MOVE_MODEL_STEP;
	}

	void moveModelFar()
	{
		// if (translation.z <= 2.5f)
			translation.z += MOVE_MODEL_STEP;
	}

	void toggleTexColor()
	{
		renderMode = renderMode == RenderMode::COLOR ? RenderMode::TEXTURE : RenderMode::COLOR;
	}

	void lumos()
	{
		lightOn = true;
	}

	void nox()
	{
		lightOn = false;
	}

	void setElder()
	{
		translation = nrg::vec3(4.2f, -0.5f, 2.1f);
		scale = nrg::vec3(0.51f, 0.51f, 0.51f);
		rotation_x = 79.001f;
		rotation_y = 65.001f;
		renderMode = RenderMode::TEXTURE;
	}
};