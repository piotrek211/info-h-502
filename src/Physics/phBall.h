#pragma once


#include "Objects/Object.h"
#include "Physics.h"
#include "PhObject.h"

class PhBall : public PhObject {
	public:
		PhBall(Physics* physics, GEngine::Object* object, float mass , float radius);
		~PhBall();
	private:
		float m_radius;



};