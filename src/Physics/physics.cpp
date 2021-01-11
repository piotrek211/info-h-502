#include "physics.h"

#include <stdio.h>

#include <BulletCollision/CollisionShapes/btTriangleMesh.h>

Physics::Physics() {
	init();
}

Physics::~Physics() {

	for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

}

void Physics::init() {

	///-----initialization_start-----

///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

	m_dynamicsWorld->setGravity(btVector3(0, -40.0f, 0));  //the gravity is set to -40.0f because of the problem with scaling. It was just easier to put bigger value for the gravity then rescaling all models.
	//m_dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}


void Physics::update(btScalar timeStep) {
	m_dynamicsWorld->stepSimulation(timeStep);
	//->stepSimulation(timeStep, MAX_SIMULATION_SUBSTEPS, FIXED_SIMULATION_TIMESTEP);
	for (int i = 0; i < m_phObjects.size(); i++) {
		m_phObjects[i]->update();
	}
}


btCollisionShape* Physics::createStaticCollisionShape(GEngine::Mesh* mesh, const btVector3& scale){

	int numIndices = mesh->getIndices()->size();
	int numVertices = mesh->getVerticesNum();
	std::cout << "HERE " << numVertices << "  " << numIndices << std::endl;
	if (numIndices < 1 || numVertices < 3)
		return NULL;
	std::vector<int>* indices1 = new std::vector<int>;
	std::vector<GLuint>* indi = mesh->getIndices();
	for (int i = 0; i < numIndices; i++) {
		indices1->push_back((int) indi->at(i));

	}

	btTriangleIndexVertexArray* tiva = new btTriangleIndexVertexArray(numIndices/3,(int*) &(indices1->at(0)), 3 * sizeof(int), numVertices, (btScalar*)(mesh->getVertices()), 14 * sizeof(float));
	if (tiva == nullptr) {
		std::cout << "NULLPTR" << std::endl;
	}
	btBvhTriangleMeshShape * shape = new btBvhTriangleMeshShape(tiva, true);
	shape->setLocalScaling(scale);
	return shape;

}

std::vector<btCollisionShape*> Physics::createStaticCollisionShapes(GEngine::Model* model, const btVector3& scale){
	std::vector<btCollisionShape*> ret;
	for (unsigned int i = 0; i < model->getMeshes()->size(); i++) {
		btCollisionShape* shape = createStaticCollisionShape(model->getMeshes()->at(i), scale);
		if (shape) {
			ret.push_back(shape);
		}
	}
	return ret;
}

std::vector<btCollisionShape*> Physics::createStaticCollisionShapes(GEngine::Model* model, float scale) {
	return createStaticCollisionShapes(model, btVector3(scale, scale, scale));
}




void Physics::addStaticModel(PhObject* phObject) {
	GEngine::Object* object = phObject->getObject();
	btTransform transform = btTransFrom(glm2bt(object->getPosition()));
	btVector3 scale = glm2bt(object->getScale());
	std::vector<btCollisionShape*>& collisionShapes = Physics::createStaticCollisionShapes(object->getObjectModel(), 1.0f);
	for (unsigned int i = 0; i < collisionShapes.size(); i++) {
		btCollisionShape* colShape;

		if (scale != btVector3(1, 1, 1)) {
			colShape = new btScaledBvhTriangleMeshShape((btBvhTriangleMeshShape*)collisionShapes[i], scale);
		} else {
			colShape = collisionShapes[i];
		}
		btRigidBody* body = addRigidBody(phObject, transform, colShape, true);
	}
}

void Physics::addDynamicModel(PhObject* phObject) {
	GEngine::Object* object = phObject->getObject();
	GEngine::Model* model = object->getObjectModel();
	GEngine::Mesh* mesh = model->getMeshes()->at(0);
	int numVertices = mesh->getVerticesNum();
	float* vertices = mesh->getVertices();
	std::vector<glm::vec4>* convexHull = mesh->getConvexHull();
	std::cout << "ORIGINAL VERTICES: " << numVertices << " VERTICES:" << convexHull->size() << "STRIDE " << sizeof(glm::vec4) << std::endl;
	btConvexHullShape* shape = new btConvexHullShape((const btScalar*)(&convexHull->at(0)), convexHull->size(), sizeof(glm::vec4));
	float scaling[4] = { 1, 1, 1, 1 };

	btVector3 localScaling(scaling[0], scaling[1], scaling[2]);
	shape->setLocalScaling(localScaling);

	btVector3 vec3 = glm2bt(object->getPosition());

	btTransform startTransform = btTransFrom(vec3);
	btRigidBody* body = addRigidBody(phObject, startTransform, shape);

}


btRigidBody* Physics::addRigidBody(PhObject* phObject, const btTransform& startTransform, btCollisionShape* shape, bool staticBody) {
	if (!staticBody) {
		m_phObjects.push_back(phObject);
	}
	btRigidBody* body = createRigidBody(phObject->getMass(), startTransform, shape);

	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodies.push_back(body);
	phObject->setRigidBody(body);
	return body;
}



btRigidBody* Physics::createRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape) {
	btVector3 localInertia(0, 0, 0);

	if (mass != 0.0f)  //dynamic if mass is not 0.0f
		shape->calculateLocalInertia(mass, localInertia);
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(0);

	return body;
}





void Physics::addGroundSurface() {
	// Infinite plane defined by normal
	btCollisionShape* shape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0, 0, 0));

	btDefaultMotionState* motion =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, motion, shape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	m_dynamicsWorld->addRigidBody(groundRigidBody);

}


btTransform Physics::btTransFrom(const btVector3& origin, const btQuaternion& rot) {
	btTransform ret;
	ret.setIdentity();
	ret.setRotation(rot);
	ret.setOrigin(origin);

	return ret;
}