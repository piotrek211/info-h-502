#pragma once

#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
namespace GEngine {
	class Object {
		public:
			Object(glm::vec3 position, glm::vec3 orientation, glm::vec3 scale = glm::vec3(1.0f), Model* model = nullptr);
			~Object();
			void updatePosition(glm::vec3 position, float angle, glm::vec3 orientation);
			void updatePosition(glm::mat4 modelMatrix);
			glm::mat4 getObjectMat() const {
				return m_objectMat;
			}
			Model* getObjectModel() const {
				return m_model;
			}
			glm::vec3 getPosition() const {
				return m_position;
			}

			glm::vec3 getPosDiff() {
				return (m_position - m_previousPosition);
			}

			glm::vec3 getScale() const {
				return m_scale;

			}

			glm::vec3 getOrientation() const {
				return m_orientation;

			}
			void setChildObject(Object* object) {
				m_childObject = object;
			}

			
			glm::mat4 getInitObjectMat() {
				return m_initObjectMat;
			}

			void setInitObjectMat(glm::mat4 initObjectMat) {
				m_initObjectMat = initObjectMat;
				//std::cout << glm::to_string(m_initObjectMat) << std::endl;
			}

			float getDeviation() {
				return m_deviation;
			}
			void addDeviation(float deviation) {
				m_deviation += deviation;
			}
			void setDeviation(float deviation) {
				m_deviation = deviation;
			}
		private:
			Model* m_model;
			glm::vec3 m_position;
			glm::vec3 m_previousPosition;
			glm::vec3 m_orientation;
			glm::vec3 m_scale;
			glm::mat4 m_objectMat = glm::mat4(1.0f);
			glm::mat4 m_initObjectMat;
			Object* m_childObject = nullptr;
			float m_deviation = 0.0f;
	};


}