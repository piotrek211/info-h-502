#pragma once


#include <iostream>

#include "btBulletDynamicsCommon.h"
#include "Mesh.h"
#include "PhObject.h"

#define FIXED_SIMULATION_TIMESTEP 1/120.f 
#define MAX_SIMULATION_SUBSTEPS   10 

class Physics {
	public:
		Physics();
		~Physics();
		void print() {

			std::cout << "dupa" << std::endl;
		}
		void addGroundSurface();
		btRigidBody* addRigidBody(PhObject* phObject, const btTransform& startTransform, btCollisionShape* shape, bool staticBody = false);
		//btRigidBody* addRigidBody(const btTransform& startTransform, btCollisionShape* shape);
		void update(btScalar timeStep);
		static btCollisionShape* createStaticCollisionShape(GEngine::Mesh* mesh, const btVector3& scale);
		static std::vector<btCollisionShape*> Physics::createStaticCollisionShapes(GEngine::Model* model, const btVector3& scale);
		void addStaticModel(PhObject* phObject);
		void addDynamicModel(PhObject* phObject);
		static std::vector<btCollisionShape*> Physics::createStaticCollisionShapes(GEngine::Model* model, float scale);
		static btTransform Physics::btTransFrom(const btVector3& origin, const btQuaternion& rot = btQuaternion(0, 0, 0));
		static btVector3 glm2bt(const glm::vec3& vec){
			return { vec.x, vec.y, vec.z };
		}


	private:
		void init();

		btRigidBody* createRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape);
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher* m_dispatcher;
		btBroadphaseInterface* m_overlappingPairCache;
		btSequentialImpulseConstraintSolver* m_solver;
		btDiscreteDynamicsWorld* m_dynamicsWorld;
		std::vector<PhObject*> m_phObjects;
		std::vector<btRigidBody*> m_rigidBodies;




};