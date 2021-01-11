#include "Mesh.h"

#include "Renderer.h"

namespace GEngine {

    Mesh::Mesh(float* vertices, uint32_t vertices_size, uint32_t vertices_count, std::vector<GLuint>* indices, std::vector<Texture*>* textures) : vertices(vertices), 
        vertices_size(vertices_size), vertices_count(vertices_count), indices(indices), textures(textures) {
		setupMesh();

	}

	void Mesh::setupMesh() {
        // Create buffers/arrays

        vertexArray = new VertexArray();
        vertexArray->bind();

        vertexBuffer = new VertexBuffer(vertices, vertices_size);
        indexBuffer = new IndexBuffer(indices);
        bufferlayout = { {ShaderDataType::Vec3, "a_Position" }, {ShaderDataType::Vec3, "a_Normal"}, {ShaderDataType::Vec2, "a_TexCoords" }, {ShaderDataType::Vec3, "a_Tangent" }, {ShaderDataType::Vec3, "a_Bitangent" } };
        vertexBuffer->setLayout(bufferlayout);

        vertexArray->addVertexBuffer(*vertexBuffer);
        vertexArray->setIndexBuffer(*indexBuffer);
        vertexArray->unbind();

	}

    void Mesh::bind(Shader& shader, unsigned int offset) {
        GLuint albedoNr = 1;
        GLuint metallicNr = 1;
        GLuint aoNr = 1;
        GLuint normalNr = 1;
        GLuint roughnessNr = 1;
        for (GLuint i = 0; i < textures->size(); i++) {
            std::string number;
            std::string name = textures->at(i)->getType();
            if (name == "texture_albedo") {
                number = std::to_string(albedoNr++);
            }
            else if (name == "texture_metallic") {
                number = std::to_string(metallicNr++);
            }
            else if (name == "texture_ao") {
                number = std::to_string(aoNr++);
            }
            else if (name == "texture_normal") {
                number = std::to_string(normalNr++);
            } else if (name == "texture_roughness") {
                number = std::to_string(roughnessNr++);
            }
            // Now set the sampler to the correct texture unit
            shader.setInt((name + number).c_str(), i+offset);
            textures->at(i)->bind(i+offset);

        }
    }

    void Mesh::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);

    }

}