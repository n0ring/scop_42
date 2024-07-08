#include "Texture.hpp"
#include <iostream>
#include "BmpLoader.hpp"

Texture::Texture(const std::string &filePath)
	: m_renderID(0), m_filePath(filePath), m_localBuffer(nullptr),
	  m_width(0), m_height(0), m_BPP(0)
{
	glGenTextures(1, &m_renderID);
	glBindTexture(GL_TEXTURE_2D, m_renderID);
	std::vector<unsigned char> data;

	// stbi_set_flip_vertically_on_load(1);
	// m_localBuffer = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_BPP, 4);
	if (BmpLoader::loadBMPTexture(m_filePath.c_str(), data, m_height, m_width) == false)
	{
		std::cout << "Error texture load. filename: " << filePath << std::endl;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// if (m_localBuffer)
	// 	stbi_image_free(m_localBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_renderID);
}

void Texture::bind(const unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_renderID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
