#pragma once

#include <string>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "vendor/stb_image/stb_image.h"
#include "BmpLoader.hpp"

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

	void bind(const unsigned int slot = 0) const;
	void unbind() const;
private:
	unsigned int m_renderID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BPP;

	inline int getWidth() {return m_width; }
	inline int getHeight() {return m_height; }
};