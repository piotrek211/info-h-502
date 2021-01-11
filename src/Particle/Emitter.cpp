#include "Emitter.h"



namespace GEngine {

	Emitter::Emitter(int particleCount) {
	    init(particleCount);
	}

	void Emitter::init(int particleCount) {

		this->particles.resize(particleCount);
		for (int i = 0; i < this->particles.size(); ++i) {
			// give every particle a random position
			this->particles[i].position = randomVec3(-1.0f, 1.0f);
			this->particles[i].lifetime = glm::linearRand(1.0f, 2.0f);
		}

		vertexArray = new VertexArray();
		vertexArray->bind();


		vertexBuffer = new VertexBuffer(cubeVertices, sizeof(cubeVertices), 36);
		positionBuffer = new VertexBuffer(this->particles.size() * 4 * sizeof(float), particleCount);

		bufferlayout = { {ShaderDataType::Vec3, "vertex_position" } };
		positionLayout = { {ShaderDataType::Vec4, "position", true} };

		vertexBuffer->setLayout(bufferlayout);
		positionBuffer->setLayout(positionLayout);

		vertexArray->addVertexBuffer(*vertexBuffer);
		vertexArray->addVertexBuffer(*positionBuffer);
		vertexArray->unbind();
	}


	void Emitter::update(const float dt, Object* object) {
		float x = (object->getPosDiff()).x / dt;
		float y = (object->getPosDiff()).y / dt;
		float z = (object->getPosDiff()).z / dt;
		float idle = y;

		if (x == 0.0f && y == 0.0f && z == 0.0f) {
			idle = 1.0f;
		}

		for (int i = 0; i < this->particles.size(); ++i) {
			// subtract from the particles lifetime
			this->particles[i].lifetime -= dt;

			// if the lifetime is below 0 respawn the particle
			if (this->particles[i].lifetime <= 0.0f)
			{
				this->particles[i].position = randomVec3(-1.0f, 1.0f);
				this->particles[i].lifetime = glm::linearRand(1.0f, 2.0f);
			}

			// move the particle depending on the velocity of the object (ball)
			this->particles[i].position += glm::vec3(dt * 0.4f * -x, dt * 2.0f * idle, dt * 0.4f * -z);

			// update the position buffer
			this->positions[i * 4 + 0] = this->particles[i].position[0];
			this->positions[i * 4 + 1] = this->particles[i].position[1];
			this->positions[i * 4 + 2] = this->particles[i].position[2];
			this->positions[i * 4 + 3] = this->particles[i].lifetime;
		}
		positionBuffer->updateData(this->positions, this->particles.size() * 4 * sizeof(float));
	}


	glm::vec3 Emitter::randomVec3(float min, float max) {
		float x = glm::linearRand(min, max);
		float y = glm::linearRand(min, max);
		float z = glm::linearRand(min, max);
		return glm::vec3(x, y, z);

	}



}