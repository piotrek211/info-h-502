#pragma once
#include <glad/glad.h>
#include <stdint.h>
#include "Buffer.h"
namespace GEngine {
	class VertexArray {
		public:
			VertexArray();
			~VertexArray();
			void bind() const;
			void unbind() const;
			void addVertexBuffer(VertexBuffer& vertexBuffer);
			void setIndexBuffer(const IndexBuffer& indexBuffer);
			IndexBuffer& getIndexBuffer() { return m_indexBuffer; }
			std::vector<VertexBuffer*>& getVertexBuffers() { return m_vertexBuffers; }
			uint32_t m_vertexBufferIndex = 0;

	private:
		uint32_t m_ID;
		std::vector<VertexBuffer*> m_vertexBuffers;
		IndexBuffer m_indexBuffer;

	};



}