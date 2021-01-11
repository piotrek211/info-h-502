#pragma once

#include "Camera.h"
#include "Projection.h"

#include "Shader.h"
#include "VertexArray.h"
#include "Log.h"
#include "Mesh.h"
#include "Model.h"
#include "Particle/Emitter.h"
namespace GEngine {

	class Renderer {
		public:
			void init();
			void end();
			void beginScene(Camera& camera, Projection& projection);
			void endScene();
			void submit(Shader& shader, const VertexArray& vertexArray, const glm::mat4& model);
			void drawSkyBox(Shader& shader,VertexArray& vertexArray, const CubeMapTexture& skyboxCube);
			void submit(Shader& shader, const glm::mat4& model);
			void onWindowResize(uint32_t width, uint32_t height);
			void setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
			void setClearColor(const glm::vec4& color);
			void clear();
			static void indexDraw(VertexArray& vertexArray);
			void draw(VertexArray& vertexArray);
			void instancedDraw(VertexArray& vertexArray, int index);
			void drawModel(Shader& shader, Model& vertexModel, const glm::mat4& model);
			void drawMesh(Shader& shader, Mesh& mesh);
			void drawParticles(Shader& shader, Emitter& emitter, Object& object, const glm::mat4& model);
			void setDeltaTime();
			void setDepth(std::string depthType);
		private:
			glm::mat4 vpMatrix = glm::mat4(1.0f);
			glm::mat3 normal = glm::mat3(1.0f);
			float deltaTime, lastFrame;
			Camera* camera;
			Projection* projection;
	};








}