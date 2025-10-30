#pragma once
#include <cstdint>
#include "glm/vec2.hpp"

struct Circle
{
	float Radius;
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
	};
};