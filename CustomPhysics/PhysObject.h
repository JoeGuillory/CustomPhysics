#pragma once
#include <glm/vec2.hpp>
#include "Shape.h"
class PhysObject
{
public:
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec2 Acceleration;
	float Mass;
	bool bUseGravity;
	
	Shape Collider;
	PhysObject();

	void TickPhys(float Delta);
	void AddForce(glm::vec2 Force);
	void AddVelocity(glm::vec2 Velocity);
	void AddAccel(glm::vec2 Accel);
	void AddImpulse(glm::vec2 Impulse);
	void SetGravity(bool Allow);
	void Draw() const;

};

