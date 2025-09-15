#pragma once

#include "core.h"
#include <vector>
#include "VertexBuffer.h"

namespace lambda {
	struct VertexArrayIndex {
		u32 Size;
		u32 Stride;
		u32 Pointer;
	};

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		VertexArray(const VertexArray& other) = delete;
		VertexArray& operator=(const VertexArray& other) = delete;

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(std::shared_ptr<VertexBuffer> buffer, const std::vector<VertexArrayIndex>& vertexArrayIndices);

		void Draw(u32 vertexCount) const;
	private:
		u32 m_VertexArrayID;
		u32 m_CurrentIndex;
	};
}