#include "mesh.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, VertexLayout layout)
{
	m_indexCount = static_cast<GLsizei>(indices.size());

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	if (layout == Position)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
		glEnableVertexAttribArray(0);
	}
	if (layout == PositionTexture)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
		glEnableVertexAttribArray(0); 
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
		glEnableVertexAttribArray(1);
	}

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	if (m_ebo != 0) glDeleteBuffers(1, &m_ebo);
	if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
	if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
}

void Mesh::Draw() const
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Mesh::DrawInstanced(GLsizei instanceCount) const
{
	glBindVertexArray(m_vao);
	glDrawElementsInstanced(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
	glBindVertexArray(0);
}