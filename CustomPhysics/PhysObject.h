#pragma once
#include <glm/vec2.hpp>
#include "Shape.h"
#include "glm/glm.hpp"
class PhysObject
{
public:
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec2 Acceleration;
	float Mass;
	bool bUseGravity;
private:
	float Pen;
public:

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

float ResolveCollision(const glm::vec2& PosA, const glm::vec2& VelA, float MassA,
	const glm::vec2& PosB, const glm::vec2& VelB, float MassB,
	float Elasticity, const glm::vec2& Normal);

void ResolvePhysObjects(PhysObject& Lhs, PhysObject& Rhs, float Elasticity, const glm::vec2& Normal, float Pen);