#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace GEngine {
	class Projection {
		public:
			glm::mat4 getProjectionMatrix() {
				return m_projectionMatrix;
			}
			void setProjectionMatrix(glm::mat4 projectionMatrix) {
				m_projectionMatrix = projectionMatrix;
			}
		private:
			glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
	};


	
	class PerspectiveProjection : public Projection {
		public:
			PerspectiveProjection(float _fov, float _aspect, float _near, float _far){
				setProjectionMatrix(glm::perspective(glm::radians(_fov), _aspect, _near, _far));
			}

	};


	class OrthographicProjection : public Projection {
		public:
			OrthographicProjection(float left, float right, float bottom, float top) {
				setProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f));
			}

	};




}