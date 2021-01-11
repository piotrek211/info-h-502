#pragma once
#include <glad/glad.h>

#include <string>
#include <vector>
namespace GEngine {
	class Texture {
		public:
			Texture(uint32_t width, uint32_t height);
			Texture(const std::string& path, const std::string& type = "");
			~Texture();
			void setData(void* data, uint32_t size);
			uint32_t getWidth() const { return m_width; }
			uint32_t getHeight() const { return m_height; }
			void bind(uint32_t slot = 0) const;
			void unbind() const;
			std::string getType() const { return m_type; }
			std::string getPath() const { return m_path; }
			uint32_t getID() const { return m_ID; }
		private:
			std::string m_type;
			std::string m_path;
			uint32_t m_width, m_height;
			uint32_t m_ID;
			GLenum m_InternalFormat, m_DataFormat;
			void* data = nullptr;
	};

	class CubeMapTexture {
		public:
			CubeMapTexture(const GLuint& width, const GLuint& height, std::string type = "depth");
			CubeMapTexture(std::vector<std::string> cube_faces);
			uint32_t getId() const {
				return m_ID;
			}
			void bind(uint32_t slot = 0) const;
			void unbind() const;
		private:
			uint32_t m_ID;

	};

}