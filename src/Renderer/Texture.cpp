#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <iostream>
#include "Log.h"
namespace GEngine {

	Texture::Texture(uint32_t width, uint32_t height) : m_width(width), m_height(height) {
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTextureStorage2D(m_ID, 1, m_InternalFormat, m_width, m_height);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	Texture::Texture(const std::string& path, const std::string& type) : m_path(path), m_type(type) {

		int width, height, channels;
	 	//stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data) {
			m_width = width;
			m_height = height;

			GLenum internalFormat = 0, dataFormat = 0;

			switch (channels) {
			case(3):
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
				break;
			case(4):
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
				break;
			default:
				break;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;


			glGenTextures(1, &m_ID);
			glBindTexture(GL_TEXTURE_2D, m_ID);


			glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(data);
		} else {
			GE_CORE_ERROR("FAILED TO LOAD TEXTURE");
			std::cout << path.c_str() << std::endl;
		}

	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_ID);
	}

	void Texture::setData(void* data, uint32_t size){
		glBindTexture(GL_TEXTURE_2D, m_ID);
		this->data = data;
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_width, m_height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void Texture::bind(uint32_t slot) const{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
	void Texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}



	CubeMapTexture::CubeMapTexture(const GLuint& width, const GLuint& height, std::string type) {
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		if (type == "depth") {
			for (unsigned int i = 0; i < 6; ++i) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			}
		} else if (type == "color") {
			for (unsigned int i = 0; i < 6; ++i) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	CubeMapTexture::CubeMapTexture(std::vector<std::string> cube_faces) {
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < cube_faces.size(); i++) {
			unsigned char* data = stbi_load(cube_faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data){
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}else{
				GE_CORE_ERROR("FAILED TO LOAD TEXTURE");
				std::cout << cube_faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}

	void CubeMapTexture::bind(uint32_t slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}
	void CubeMapTexture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}


}