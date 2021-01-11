#pragma once

#include <vector>
#include "VertexArray.h"
#include "Objects/Object.h"
#include "glm/ext.hpp"
namespace GEngine {

	class Emitter {
	public:
		Emitter(int particleCount);
		~Emitter();

		void update(const float dt, Object* object);
        glm::vec3 randomVec3(float min, float max);
		
		VertexArray* getVertexArray() const {
			return vertexArray;
		}
	private:
		void init(int particleCount);

		struct Particle {
			glm::vec3 position;  //12 bytes
			float lifetime;    //4 bytes
		};



		std::vector<Particle> particles;
		float positions[4000];
		//std::vector<Particle*> particles;
		VertexArray* vertexArray;    //VAO
		VertexBuffer *vertexBuffer, *positionBuffer;  //VBO
		IndexBuffer* indexBuffer;    //EBO
		VtxBufLayout bufferlayout, positionLayout;
		glm::vec3 translations[1000];

        float cubeVertices[108] = {
            // positions       
            -0.5f, -0.5f, -0.5f, 
             0.5f, -0.5f, -0.5f, 
             0.5f,  0.5f, -0.5f, 
             0.5f,  0.5f, -0.5f, 
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,  
             0.5f, -0.5f,  0.5f, 
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f, 
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f,  0.5f, 
            -0.5f,  0.5f,  0.5f, 

             0.5f,  0.5f,  0.5f, 
             0.5f,  0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f, 
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  

            -0.5f, -0.5f, -0.5f, 
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f, 
             0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f, -0.5f,  

            -0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f, 
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f, -0.5f
        };






	};



} 