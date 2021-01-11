#pragma once

#include "Objects/Object.h"
#include "btBulletDynamicsCommon.h"
class PhObject {
	public:
		PhObject(GEngine::Object* object, float mass, std::string type= "");
		~PhObject();
		float getMass() const {
			return m_mass;
		}
		GEngine::Object* getObject() const {
			return m_object;
		}
		void setRigidBody(btRigidBody* rigidBody) {
			m_rigidBody = rigidBody;
		}
		btRigidBody* getRigidBody() const {
			return m_rigidBody;
		}

		void launch(float force, glm::vec3 direction);

		void setPhPosition(btTransform transform);
		void moveUp();

		void move(std::string direction);

		void reset();

		void update();

	protected:
		float m_mass;
		GEngine::Object* m_object;
		btRigidBody* m_rigidBody;
		std::string m_type;
		btTransform m_transform;
		btTransform m_trans;
};