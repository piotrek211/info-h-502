#pragma once
#include <glad/glad.h>
#include <stdint.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define VERTEX_SHADER GL_VERTEX_SHADER
#define FRAGMENT_SHADER GL_FRAGMENT_SHADER
#define GEOMETRY_SHADER GL_GEOMETRY_SHADER
namespace GEngine {
	class Shader {
		public:
			Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath, const GLchar* geometryShaderPath = nullptr);
			~Shader();
			void bind() const;
			void unbind() const;

			uint32_t getOffset() const { return offset; }
			void setOffset(uint32_t offset) {
				this->offset = offset;
			}

			void setInt(const std::string& name, int value);
			void setIntArray(const std::string& name, int* values, uint32_t count);

			void setFloat(const std::string& name, float value);
			void setFloat3(const std::string& name, const glm::vec3& value);
			void setFloat4(const std::string& name, const glm::vec4& value);

			void setMat4(const std::string& name, const glm::mat4& value);

			void uploadUniformInt(const std::string& name, int value);

			void uploadUniformIntArray(const std::string& name, int* values, uint32_t count);

			void uploadUniformFloat(const std::string& name, float value);

			void uploadUniformFloat2(const std::string& name, const glm::vec2& value);

			void uploadUniformFloat3(const std::string& name, const glm::vec3& value);

			void uploadUniformFloat4(const std::string& name, const glm::vec4& value);

			void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);

			void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		private:
			GLuint getShader(const GLchar* shaderPath, GLenum shaderType);
			void checkErrors(const GLuint& object, const uint8_t type);
			uint32_t m_ID;
			uint32_t offset = 0;
	};


}