#pragma once
#include <glad/glad.h>
#include <stdint.h>

#include <string>
#include <vector>
#include <iostream>
namespace GEngine {


	enum class ShaderDataType {
			None = 0, Vec, Vec2, Vec3, Vec4

	};

	static std::pair<std::vector<uint32_t>, GLenum>* ShaderDataTypeSize(ShaderDataType type){
		uint32_t size;
		std::vector<uint32_t> res;
		std::pair<std::vector<uint32_t>, GLenum>* result = new std::pair<std::vector<uint32_t>, GLenum>;
		GLenum OpenGlType = GL_FLOAT;
		switch (type){
			case ShaderDataType::Vec:    
				size = sizeof(GL_FLOAT);
				res.push_back(1);    //count
				res.push_back(size); //size
				OpenGlType = GL_FLOAT;
				break;
			case ShaderDataType::Vec2:
				size = 2 * sizeof(GL_FLOAT);
				res.push_back(2);
				res.push_back(size);
				OpenGlType = GL_FLOAT;
				break;
			case ShaderDataType::Vec3:
				size = 3 * sizeof(GL_FLOAT);
				res.push_back(3);
				res.push_back(size);
				OpenGlType = GL_FLOAT;
				break;
			case ShaderDataType::Vec4:
				size = 4 * sizeof(GL_FLOAT);
				res.push_back(4);
				res.push_back(size);
				OpenGlType = GL_FLOAT;
				break;
		}
		result->first = res;
		result->second = OpenGlType;
		return result;
	}

	struct BufferElement {
		std::string name;
		GLenum type;
		uint32_t size;
		uint32_t elementCount;
		size_t offset;
		GLboolean normalized;
		GLboolean instanced;

		BufferElement(ShaderDataType type, const std::string& name, bool instanced = false, bool normalized = false): name(name),
					offset(0), instanced((GLboolean) instanced), normalized((GLboolean) normalized) {
			std::pair<std::vector<uint32_t>, GLenum>* res = ShaderDataTypeSize(type);
			this->elementCount = (res->first).at(0);
			this->size = (res->first).at(1);
			this->type = res->second;
			delete res;
		};
	
	};
	
	class VtxBufLayout {
		public:
			VtxBufLayout() = default;
			VtxBufLayout(std::initializer_list<BufferElement> elements) : m_elements(elements) { setStrideOffset();  };
			const std::vector<BufferElement>& getElements() const { return m_elements; };
			const uint32_t getStride() { return m_stride; }
		private:
			void setStrideOffset() {
				uint32_t offset = 0;
				for (auto& element : m_elements) {
					element.offset = offset;
					offset += element.size;
				}
				m_stride = offset;
			}
			std::vector<BufferElement> m_elements;
			uint32_t m_stride = 0;

	};
	
	
	class VertexBuffer {
		public:
			void bind() const;
			void unBind() const;
			VertexBuffer() = default;
			VertexBuffer(uint32_t size, uint32_t count = 0);
			VertexBuffer(float* vertices, uint32_t size, uint32_t count = 0);
			void updateData(const void* data, uint32_t size);
			const VtxBufLayout& getLayout() const { return m_layout; };
			void setLayout(const VtxBufLayout& layout) { this->m_layout = layout; };
			uint32_t getCount() const { return m_count; };
			~VertexBuffer();
		private:
			uint32_t m_count;
			VtxBufLayout m_layout;
			uint32_t m_ID;

	};



	class IndexBuffer {
		public:
			void bind() const;
			void unBind() const;
			IndexBuffer() = default;
			IndexBuffer(std::vector<uint32_t>* indices) : IndexBuffer(&(indices->at(0)), indices->size() * sizeof(uint32_t)) {};
			IndexBuffer(uint32_t* indices, uint32_t count);
			~IndexBuffer();
			uint32_t getCount() const { return m_count; }
		private:
			uint32_t m_ID;
			uint32_t m_count;

	};

}