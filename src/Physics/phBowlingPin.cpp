#include "phBowlingPin.h"


//PhBall::PhBall(Physics* physics, GEngine::Object* object, float mass, float radius) : PhObject(physics, object, mass) {
PhBowlingPin::PhBowlingPin(Physics* physics, GEngine::Object* object, float mass, float radius) : PhObject(object, mass), m_radius(radius) {
	glm::vec3 position = object->getPosition();
	glm::vec3 scale = object->getScale();
	// Create collision shape
	btCollisionShape* collisionShape = new btSphereShape(radius * scale.x);

	// Motion state
	glm::quat orientation(0.0f, 0.0f, 0.0f, 1.0f);
	btTransform startTransform = btTransform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w),
		btVector3(position.x, position.y, position.z));


	m_rigidBody = physics->addRigidBody(this, startTransform, collisionShape);
	setRigidBody(m_rigidBody);

}