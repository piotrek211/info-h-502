#include "Buffer.h"

namespace GEngine {

	VertexBuffer::VertexBuffer(uint32_t size, uint32_t count): m_count(count) {
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	}

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size, uint32_t count): m_count(count) {
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	}

	void VertexBuffer::updateData(const void* data, uint32_t size) {
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}


	void VertexBuffer::bind() const{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);

	}

	void VertexBuffer::unBind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_ID);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) {
		m_count = count;
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, GL_STATIC_DRAW);
	}


	void IndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void IndexBuffer::unBind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_ID);

	}



}