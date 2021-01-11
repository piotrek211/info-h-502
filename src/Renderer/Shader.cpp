#include "Shader.h"
#include "Log.h"
#include <vector>
#include "utils.h"
namespace GEngine {

	Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath, const GLchar* geometryShaderPath) {

		GLuint program_id = glCreateProgram();
		GLuint vertexShader = getShader(vertexShaderPath, VERTEX_SHADER);
	 	GLuint fragmentShader = getShader(fragmentShaderPath, FRAGMENT_SHADER);
		GLuint geometryShader;
		// check for shader compile errors
		checkErrors(vertexShader, 1);
		checkErrors(fragmentShader, 1);

		glAttachShader(program_id, vertexShader);
		glAttachShader(program_id, fragmentShader);

		if (geometryShaderPath != nullptr) {
			geometryShader = getShader(geometryShaderPath, GEOMETRY_SHADER);
			checkErrors(geometryShader, 1);
			glAttachShader(program_id, geometryShader);
		}

		glLinkProgram(program_id);
		checkErrors(program_id, 0);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		if (geometryShaderPath != nullptr) {
			glDeleteShader(geometryShader);
		}
		
		m_ID = program_id;


	}

	Shader::~Shader() {
		glDeleteProgram(m_ID);

	}

	GLuint Shader::getShader(const GLchar* shaderPath, GLenum shaderType){
		std::string shaderStr = GEngine::Utils::open(shaderPath);
		const GLchar* shaderSource = shaderStr.c_str();
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);
		return shader;
	}

	void Shader::checkErrors(const GLuint& object, const uint8_t type) {
		GLint success = 0;
		switch (type) {
			case 0:
				glGetProgramiv(object, GL_LINK_STATUS, &success);
				if (success == GL_FALSE) {
					GLint maxLength = 0;
					glGetProgramiv(object, GL_INFO_LOG_LENGTH, &maxLength);

					std::vector<GLchar> infoLog(maxLength);
					glGetShaderInfoLog(object, maxLength, &maxLength, &infoLog[0]);
					GE_CORE_ERROR("{0}", infoLog.data());
				}
				break;

			case 1:
				glGetShaderiv(object, GL_COMPILE_STATUS, &success);
				if (success == GL_FALSE) {
					GLint maxLength = 0;
					glGetShaderiv(object, GL_INFO_LOG_LENGTH, &maxLength);

					std::vector<GLchar> infoLog(maxLength);
					glGetShaderInfoLog(object, maxLength, &maxLength, &infoLog[0]);
					GE_CORE_ERROR("{0}", infoLog.data());
				}
				break;
		}

	}

	void Shader::bind() const {
		glUseProgram(m_ID);
	}

	void Shader::unbind() const {
		glUseProgram(0);
	}


	void Shader::setInt(const std::string& name, int value){
		uploadUniformInt(name, value);
	}

	void Shader::setIntArray(const std::string& name, int* values, uint32_t count){
		uploadUniformIntArray(name, values, count);
	}

	void Shader::setFloat(const std::string& name, float value){
		uploadUniformFloat(name, value);
	}

	void Shader::setFloat3(const std::string& name, const glm::vec3& value){
		uploadUniformFloat3(name, value);
	}

	void Shader::setFloat4(const std::string& name, const glm::vec4& value){
		uploadUniformFloat4(name, value);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& value) {
		uploadUniformMat4(name, value);
	}

	void Shader::uploadUniformInt(const std::string& name, int value){
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void Shader::uploadUniformIntArray(const std::string& name, int* values, uint32_t count){
		glUniform1iv(glGetUniformLocation(m_ID, name.c_str()), count, values);
	}

	void Shader::uploadUniformFloat(const std::string& name, float value){
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void Shader::uploadUniformFloat2(const std::string& name, const glm::vec2& value){
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y);
	}

	void Shader::uploadUniformFloat3(const std::string& name, const glm::vec3& value){
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
	}

	void Shader::uploadUniformFloat4(const std::string& name, const glm::vec4& value){
		glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w);
	}

	void Shader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix){
		glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix){
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}



}