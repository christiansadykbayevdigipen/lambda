#pragma once

#include "core.h"
#include <vector>

namespace lambda {
	class VertexBuffer {
	public:
		VertexBuffer(const std::vector<f32>& vertices);
		VertexBuffer();

		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;

		~VertexBuffer();

		void FillData(const std::vector<f32>& vertices);

		void Bind() const;
		void Unbind() const;
	private:
		u32 m_VertexBufferID;
	};
}