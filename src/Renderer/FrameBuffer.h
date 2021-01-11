#pragma once


#include <string>
namespace GEngine {
	class FrameBuffer {
		public:
			FrameBuffer() = default;
			FrameBuffer(const GLuint& width, const GLuint& height, std::string type = "depth");
			FrameBuffer(const GLuint& width, const GLuint& height);
			~FrameBuffer();
			void bind();
			void unbind();
			CubeMapTexture* getCubeMapTexture() {
				return cubeMap;
			}
			std::string getType() {
				return m_type;
			}
		private:
			uint32_t m_ID;
			GLuint m_width;
			GLuint m_height;
			CubeMapTexture* cubeMap = nullptr;
			std::string m_type = "depth";

	};


}