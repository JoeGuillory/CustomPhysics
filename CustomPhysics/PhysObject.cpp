#include "PhysObject.h"
#include "raylib.h"
PhysObject::PhysObject() : Position({0,0}), Velocity({0,0}), Acceleration({0,0}), Mass(1.0f), bUseGravity(true)
{
}

void PhysObject::TickPhys(float Delta)
{
	Velocity += Acceleration * Delta;
	Position += Velocity * Delta;
	Acceleration = { 0,0 };


}

void PhysObject::AddForce(glm::vec2 Force)
{
	Acceleration += Force / Mass;
}

void PhysObject::AddVelocity(glm::vec2 Velocity)
{
	this->Velocity += Velocity;
}

void PhysObject::AddAccel(glm::vec2 Accel)
{
	Acceleration += Accel;
}

void PhysObject::AddImpulse(glm::vec2 Impulse)
{

	Velocity += Impulse;
}

void PhysObject::SetGravity(bool Allow)
{
	bUseGravity = Allow;
}

void PhysObject::Draw() const
{
	switch (Collider.Type)
	{
	case ShapeType::NONE:
		DrawPixel(Position.x, Position.y, RED);
		break;
	case ShapeType::CIRCLE:
		DrawCircle(Position.x, Position.y, Collider.CircleData.Radius, RED);
		break;
	case ShapeType::AABB:
		DrawRectangle(Position.x + Collider.AABBData.x, Position.y + Collider.AABBData.y, Collider.AABBData.width, Collider.AABBData.height, RED);
		break;
	default:
		break;
	}
}

float ResolveCollision(const glm::vec2& PosA, const glm::vec2& VelA, float MassA, const glm::vec2& PosB, const glm::vec2& VelB, float MassB, float Elasticity, const glm::vec2& Normal)
{
	// calculate the relative velocity
	glm::vec2 RelVel = VelB - VelA;

	// calculate the impulse magnitude
	float ImpulseMag = glm::dot(-(1.0f + Elasticity) * RelVel, Normal) /
		glm::dot(Normal, Normal * (1 / MassA + 1 / MassB));

	// return impulse to apply to both objects
	return ImpulseMag;
}

void ResolvePhysObjects(PhysObject& Lhs, PhysObject& Rhs, float Elasticity, const glm::vec2& Normal, float Pen)
{
	// calculate resolution impulse
 //   normal and pen are passed by reference and will be updated
	float ImpulseMag = ResolveCollision(Lhs.Position, Lhs.Velocity, Lhs.Mass, Rhs.Position, Rhs.Velocity, Rhs.Mass, Elasticity, Normal);

	// depenetrate objects 
	glm::vec2 Mtv = Normal * Pen;
	Lhs.Position -= Mtv;
	Rhs.Position += Mtv;

	// TODO: don't bother applying impulses to static/kinematic objects

	// apply impulses to update velocity after collision
	// remember: apply an equal but opposite force to the other
	glm::vec2 Impulse = Normal * ImpulseMag;
	Lhs.AddImpulse(-Impulse);
	Rhs.AddImpulse(Impulse);
}
