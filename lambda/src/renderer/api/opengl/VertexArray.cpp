#include "VertexArray.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace lambda {
	VertexArray::VertexArray(): m_VertexArrayID(0), m_CurrentIndex(0) {
		glGenVertexArrays(1, &m_VertexArrayID);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void VertexArray::Bind() const {
		glBindVertexArray(m_VertexArrayID);
	}

	void VertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	// TODO: Thoroughly test this code. I have no clue if this even works.
	void VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> buffer, const std::vector<VertexArrayIndex>& vertexArrayIndices) {
		buffer->Bind();
		Bind();

		for(const auto& index : vertexArrayIndices) {
			glVertexAttribPointer(m_CurrentIndex, index.Size, GL_FLOAT, GL_FALSE, index.Stride, reinterpret_cast<const void*>(index.Pointer));
			m_CurrentIndex++;
		}

		Unbind();
		buffer->Unbind();
	}

	void VertexArray::Draw(u32 vertexCount) const {
		Bind();
		for(u32 i = 0; i < m_CurrentIndex; i++) {
			glEnableVertexAttribArray(i);
		}

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		for(u32 i = 0; i < m_CurrentIndex; i++) {
			glDisableVertexAttribArray(i);
		}
		Unbind();
	}
}