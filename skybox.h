#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Skybox
{
private:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_textureID = 0;

	static GLuint LoadCubemap(const std::vector<std::string>& faces);
public:
	Skybox(const std::vector<std::string>& faces);
	~Skybox();

	void Draw() const;
};