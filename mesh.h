#pragma once

#include <vector>
#include <glad/glad.h>

enum VertexLayout
{
	Position, PositionTexture
};

class Mesh
{
private:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;
	GLsizei m_indexCount = 0;
public:
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, VertexLayout layout);
	~Mesh();

	void Draw() const;
	void DrawInstanced(GLsizei instanceCount) const;
};
