#include "VertexArray.h"

#include <iostream>
namespace GEngine {
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_ID);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_ID);


	}

	void VertexArray::bind() const {
		glBindVertexArray(m_ID);
	}

	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void VertexArray::addVertexBuffer(VertexBuffer& vertexBuffer){
		this->bind();
		vertexBuffer.bind();

		VtxBufLayout layout = vertexBuffer.getLayout();
		const std::vector<BufferElement> elements = layout.getElements();


		for (const BufferElement& element : elements) {
			glVertexAttribPointer(m_vertexBufferIndex, element.elementCount, element.type, element.normalized, layout.getStride(), (const void*) element.offset);
			glEnableVertexAttribArray(m_vertexBufferIndex);
			if (element.instanced) {
				glVertexAttribDivisor(m_vertexBufferIndex, 1);
			}
			m_vertexBufferIndex++;
		}
		m_vertexBuffers.push_back(&vertexBuffer);
		vertexBuffer.unBind();
		this->unbind();
	}


	void VertexArray::setIndexBuffer(const IndexBuffer& indexBuffer){
		glBindVertexArray(m_ID);
		indexBuffer.bind();
		m_indexBuffer = indexBuffer;
	}

}