#include "Renderer.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"

namespace GEngine {

	void Renderer::init(){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		// timing
		deltaTime = 0.0f;	// time between current frame and last frame
		lastFrame = 0.0f;
	
	}
	void Renderer::end(){}
	void Renderer::beginScene(Camera& camera, Projection& projection) {
		vpMatrix = projection.getProjectionMatrix()*camera.GetViewMatrix();
		this->camera = &camera;
		this->projection = &projection;
	}
	void Renderer::endScene(){}
	void Renderer::submit(Shader& shader, const VertexArray& vertexArray, const glm::mat4& model) {
		shader.bind();
		shader.setMat4("u_ViewProjection", vpMatrix);
		shader.setMat4("u_Model", model);
		glm::mat4 normal = glm::mat4(glm::transpose(glm::inverse(model))); //transpose of the inverse of M matrix (transpose(M^-1))
		shader.setFloat3("u_ViewPos", this->camera->getPosition());
		shader.setMat4("u_Normal", normal);
		vertexArray.bind();
	}
	void Renderer::drawSkyBox(Shader& shader, VertexArray& vertexArray, const CubeMapTexture& skyboxCube) {
		glDepthFunc(GL_LEQUAL);
		shader.bind();
		glm::mat4 vpMatrixSkybBox = projection->getProjectionMatrix() * glm::mat4(glm::mat3(camera->GetViewMatrix())); //we remove the translation for the skybox
		shader.setMat4("u_ViewProjection", vpMatrixSkybBox);
		vertexArray.bind();
		skyboxCube.bind();
		draw(vertexArray);
		glDepthFunc(GL_LESS);
	}
	void Renderer::submit(Shader& shader, const glm::mat4& model) {
		shader.bind();
		shader.setMat4("u_ViewProjection", vpMatrix);
		shader.setMat4("u_Model", model);
		glm::mat4 normal = glm::mat4(glm::transpose(glm::inverse(model)));
		shader.setFloat3("u_ViewPos", this->camera->getPosition());
		shader.setMat4("u_Normal", normal);
	}


	void Renderer::drawModel(Shader& shader, Model& vertexModel, const glm::mat4& model) {
		submit(shader, model);
		std::vector<Mesh*>* meshes = vertexModel.getMeshes();
		for (GLuint i = 0; i < meshes->size(); i++) {
			drawMesh(shader, *(meshes->at(i)));
		}
	}

	void Renderer::drawMesh(Shader& shader, Mesh& mesh) {
		mesh.bind(shader, shader.getOffset());
		VertexArray* vertexArray = mesh.getVertexArray();
		vertexArray->bind();
		indexDraw(*vertexArray);
		vertexArray->unbind();
		mesh.unbind();
	}

	void Renderer::drawParticles(Shader& shader, Emitter& emitter, Object& object, const glm::mat4& model) {
		shader.bind();
		shader.setMat4("u_ViewProjection", vpMatrix);
		shader.setMat4("u_Model", model);
		shader.setFloat3("u_objectPos", object.getPosition());
		shader.setFloat3("u_velocity", object.getPosDiff());
		VertexArray* vertexArray = emitter.getVertexArray();
		instancedDraw(*vertexArray, 1);


	}

	void Renderer::setDeltaTime() {
		if (camera != nullptr) {
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			this->camera->setDeltaTime(deltaTime);

		}

	}

	void Renderer::setDepth(std::string depthType) {
		if (depthType == "equal") {
			glDepthFunc(GL_LEQUAL);
		}
		else if (depthType == "less") {
			glDepthFunc(GL_LESS);
		}

	}




	void Renderer::onWindowResize(uint32_t width, uint32_t height) {
		setViewPort(0, 0, width, height);
	}

	void Renderer::setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);

	}
	void Renderer::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	

	void Renderer::indexDraw(VertexArray& vertexArray) {
		vertexArray.bind();
		int indexCount = vertexArray.getIndexBuffer().getCount();
		//std::cout << indexCount << std::endl;
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
		vertexArray.unbind();
	}

	void Renderer::draw(VertexArray& vertexArray) {
		vertexArray.bind();
		int count = vertexArray.getVertexBuffers().back()->getCount();
		if (count != 0) {
			glDrawArrays(GL_TRIANGLES, 0, count);
		}
		else {
			GE_CORE_WARN("Number of vertices is equal 0!");
		}
		vertexArray.unbind();
	}

	void Renderer::instancedDraw(VertexArray& vertexArray, int index) {
		int primCount = vertexArray.getVertexBuffers().at(index)->getCount();
		int cubeCount = vertexArray.getVertexBuffers().at(0)->getCount();
		vertexArray.bind();
		glDrawArraysInstanced(GL_TRIANGLES, 0, cubeCount, primCount); // 1000 cubes of 36 vertices each
		vertexArray.unbind();


	}



}