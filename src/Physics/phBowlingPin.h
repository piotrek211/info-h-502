#pragma once

#include "Objects/Object.h"
#include "Physics.h"
#include "PhObject.h"

class PhBowlingPin : public PhObject {
public:
	PhBowlingPin(Physics* physics, GEngine::Object* object, float mass, float radius);
	~PhBowlingPin();
private:
	float m_radius;



};