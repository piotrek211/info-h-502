
#include "Texture.h"
#include "FrameBuffer.h"

namespace GEngine {


    FrameBuffer::FrameBuffer(const GLuint& width, const GLuint& height, std::string type) : m_width(width), m_height(height), m_type(type){
        glGenFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        cubeMap = new CubeMapTexture(width, height, type);
        GLuint cubeMapId = (*cubeMap).getId();
        if (type == "depth") {
            // attach depth texture as FBO's depth buffer
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeMapId, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        } else if (type == "color") {
            // attach depth texture as FBO's color buffer
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeMapId, 0);
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    FrameBuffer::FrameBuffer(const GLuint& width, const GLuint& height) : m_width(width), m_height(height) {
        glGenFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    
    }


    void FrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        glViewport(0, 0, m_width, m_height);

    }

    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &m_ID);

    }


}