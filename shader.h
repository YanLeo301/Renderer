#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
private:
	GLuint m_ID;
public:
	explicit Shader(const std::string& filepath);
	~Shader();

	void Use() const;

	GLuint GetID() const;

	GLint GetUniformLocation(const std::string& name) const;

	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
};