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
		DrawRectangle(Collider.AABBData.x, Collider.AABBData.y, Collider.AABBData.width, Collider.AABBData.height, RED);
		break;
	default:
		break;
	}
}
