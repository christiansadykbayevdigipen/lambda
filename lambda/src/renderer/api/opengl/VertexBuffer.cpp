#include "VertexBuffer.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace lambda {
	VertexBuffer::VertexBuffer(const std::vector<f32>& vertices): m_VertexBufferID(0) {
		glGenBuffers(1, &m_VertexBufferID);
	
		FillData(vertices);
	}

	VertexBuffer::VertexBuffer() {
		glGenBuffers(1, &m_VertexBufferID);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_VertexBufferID);
	}

	void VertexBuffer::FillData(const std::vector<f32>& vertices) {
		Bind();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(f32), vertices.data(), GL_STATIC_DRAW);
		Unbind();
	}

	void VertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}

	void VertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}