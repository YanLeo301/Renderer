#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
private:
	GLuint m_textureID;
public:
	Texture(const char* path);
	~Texture();

	void Bind();
	void Unbind();
};