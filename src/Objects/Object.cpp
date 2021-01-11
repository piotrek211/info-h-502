#include "Object.h"



namespace GEngine {

	Object::Object(glm::vec3 position, glm::vec3 orientation, glm::vec3 scale, Model* model) : m_position(position)
	, m_orientation(orientation), m_scale(scale), m_model(model){
		updatePosition(position, 0.0f, orientation);
		//m_initObjectMat = m_objectMat;
		//std::cout << glm::to_string(m_initObjectMat) << std::endl;
	}

	Object::~Object() {
		delete m_model; 
	}

	void Object::updatePosition(glm::vec3 position, float angle, glm::vec3 orientation) {
		m_objectMat = glm::mat4(1.0f);
		m_objectMat = glm::translate(m_objectMat, position);
		//m_objectMat = glm::rotate(m_objectMat, angle, orientation);
		m_objectMat = glm::scale(m_objectMat, m_scale);
		if (m_childObject != nullptr) {
			m_childObject->updatePosition(position, angle, orientation);
		}
		m_previousPosition = position;

	}

	void Object::updatePosition(glm::mat4 objectMat) {
		m_objectMat = objectMat;
		m_previousPosition = m_position;
		m_position = glm::vec3(m_objectMat[3]);
		//std::cout << "position x: " << m_position.x << " y: " << m_position.y << " z: " << m_position.z << std::endl;
		m_objectMat = glm::scale(m_objectMat, m_scale);
		if (m_childObject != nullptr) {
			m_childObject->updatePosition(objectMat);
		}
	}





}