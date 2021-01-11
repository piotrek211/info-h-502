#pragma once
#include <string>
#include <vector>
// GL Includes
#include <glad/glad.h>
#include <glm/glm.hpp>

//

#include "VertexArray.h"
#include "Shader.h";
#include <string>
#include <vector>
#include "Texture.h"
namespace GEngine {
    class Mesh {
    public:
        Mesh(float* vertices, uint32_t vertices_size, uint32_t vertices_count, std::vector<GLuint>* indices, std::vector<Texture*>* textures);
        
        void bind(Shader& shader, unsigned int offset = 0);
        void unbind();
        float* getVertices() const {
            return vertices;
        }
        uint32_t getVerticesSize() const {
            return vertices_size;
        }

        uint32_t getVerticesNum() const {
            return vertices_count;

        }

        std::vector<GLuint>* getIndices() const {
            return indices;
        }
        std::vector<Texture*>* getTextures() const {
            return textures;
        }
        VertexArray* getVertexArray() const {
            return vertexArray;
        }

        void setConvexHull(std::vector<glm::vec4>* convexHull) {
            m_convexHull = convexHull;
        }

        std::vector<glm::vec4>* getConvexHull() {
            return m_convexHull;
        }

    private:
        /*  Mesh Data  */
        float* vertices;
        uint32_t vertices_size;
        uint32_t vertices_count;
        std::vector<GLuint>* indices;
        std::vector<Texture*>* textures;
        std::vector<glm::vec4>* m_convexHull = nullptr;
        VertexArray* vertexArray;    //VAO
        VertexBuffer* vertexBuffer;  //VBO
        IndexBuffer* indexBuffer;    //EBO
        VtxBufLayout bufferlayout;
        void setupMesh();

    };
}