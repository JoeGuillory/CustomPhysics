#pragma once
#include <cstdint>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
struct Circle
{
	float Radius;
};

struct AABB
{
	float x;
	float y;
	float width;
	float height;
};

enum class ShapeType : uint8_t
{
	NONE = 0,
	CIRCLE = 1 << 0,
	AABB = 1 << 1
};

struct Shape
{
	ShapeType Type;

	union
	{
		Circle CircleData;
		AABB   AABBData;
	};
};
bool CheckCircleCircle(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB);
bool CheckAABBAABB(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB);
bool CheckCircleAABB(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB);

bool CheckCircleCircle(const glm::vec2& PosA, const Circle& CircleA, const glm::vec2& PosB, const Circle& CircleB);
bool CheckCircleAABB(const glm::vec2& PosA, const Circle& Circle, const glm::vec2& PosB, const AABB& Box);
bool CheckAABBAABB(const glm::vec2& PosA,const AABB& BoxA, const glm::vec2& PosB, const AABB& BoxB);

glm::vec2 DepenetrateCircleCircle(const glm::vec2& PosA, const Circle& CircleA, const glm::vec2& PosB, const Circle& CircleB, float& Pen);
glm::vec2 DepenetrateCircleCircle(const glm::vec2& PosA, const Shape& CircleA, const glm::vec2& PosB, const Shape& CircleB, float& Pen);