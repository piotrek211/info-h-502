#include "PhObject.h"

#include <glm/gtx/string_cast.hpp>

PhObject::PhObject(GEngine::Object* object, float mass, std::string type) : m_object(object), m_mass(mass), m_rigidBody(nullptr) { 
	m_type = type; 

	glm::vec3 position = object->getPosition();
	glm::vec3 scale = object->getScale();

	glm::quat orientation(0.0f, 0.0f, 0.0f, 1.0f);
	btTransform startTransform = btTransform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w),
		btVector3(position.x, position.y, position.z));

	glm::mat4 mat;
	startTransform.getOpenGLMatrix(glm::value_ptr(mat));
	object->setInitObjectMat(mat);
	m_trans = startTransform;
}

PhObject::~PhObject() {

}



void PhObject::move(std::string direction) {

	glm::vec3 position = m_object->getPosition();
	float deviation = m_object->getDeviation();
	if (direction == "left" && deviation >= -5.0f) {
		position.x -= 1.0f;
		m_object->addDeviation(-1.0f);
	}
	else if (direction == "right" && deviation <= 5.0f) {
		position.x += 1.0f;
		m_object->addDeviation(1.0f);
	}

	glm::mat4 mat = m_object->getObjectMat();
	glm::vec4 pos = glm::vec4(position, 1.0f);
	mat[3] = pos;
	glm::vec3 scale = m_object->getScale();
	glm::vec3 reverseScale = glm::vec3(1/scale.x, 1/scale.y, 1/scale.z);
	mat = glm::scale(mat, reverseScale);
	btTransform trans;
	trans.setFromOpenGLMatrix(glm::value_ptr(mat));

	glm::mat4 m;
	m_transform.getOpenGLMatrix(glm::value_ptr(m));
	m_rigidBody->btCollisionObject::activate();
	m_rigidBody->setWorldTransform(trans);
	m_rigidBody->getMotionState()->setWorldTransform(trans);
	m_rigidBody->setAngularVelocity(btVector3(0, 0, 0));
	m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));

}


void PhObject::moveUp() {

	btTransform trans;


	glm::mat4 m_objectMat = glm::mat4(1.0f);
	glm::vec3 position = m_object->getPosition();
	std::cout << position.y << std::endl;
	glm::vec3 m_scale = m_object->getScale();
	position.y = position.y + 1.0f;
	m_objectMat = glm::translate(m_objectMat, position);
	trans.setFromOpenGLMatrix(glm::value_ptr(m_objectMat));
	m_object->updatePosition(m_objectMat);
	m_rigidBody->btCollisionObject::activate();
	m_rigidBody->setWorldTransform(trans);
	m_rigidBody->setAngularVelocity(btVector3(0, 0, 0));
	m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));



}


void PhObject::reset() {
	glm::mat4 mat = m_object->getInitObjectMat();
	btTransform trans;
	if (m_type == "pin") {
		mat[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		mat[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		mat[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	}

	trans.setFromOpenGLMatrix(glm::value_ptr(mat));
	m_object->setDeviation(0.0f);

	m_rigidBody->btCollisionObject::activate();
	m_rigidBody->setWorldTransform(trans);
	m_rigidBody->getMotionState()->setWorldTransform(trans);
	m_rigidBody->setAngularVelocity(btVector3(0, 0, 0));
	m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));


}

void PhObject::launch(float force, glm::vec3 direction) {
	m_rigidBody->btCollisionObject::activate();
	m_rigidBody->setAngularVelocity(btVector3(0, 0, 0));
	m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));

	m_rigidBody->applyCentralImpulse(btVector3(direction.x, direction.y, direction.z) * force);


}



void PhObject::setPhPosition(btTransform transform) {
	glm::mat4 objectMat;
	m_rigidBody->getMotionState()->setWorldTransform(transform);
	transform.getOpenGLMatrix(glm::value_ptr(objectMat));
	m_object->updatePosition(objectMat);
}

void PhObject::update() {
	if (m_rigidBody != nullptr) {
		btTransform transform;
		glm::mat4 objectMat;
		m_rigidBody->getMotionState()->getWorldTransform(transform);
		transform.getOpenGLMatrix(glm::value_ptr(objectMat));
		m_transform = transform;
		m_object->updatePosition(objectMat);
	}

}