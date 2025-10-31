#include "Shape.h"
#include <cmath>
bool CheckCircleCircle(const glm::vec2& PosA, const Circle& CircleA, const glm::vec2& PosB, const Circle& CircleB)
{
    
    float distance = std::sqrt((PosA.x * PosB.x) + (PosA.y * PosB.y));

    if (distance <= CircleA.Radius + CircleB.Radius)
    {
        return true;
    }
    return false;
}

bool CheckCircleAABB(const glm::vec2& PosA, const Circle& Circle, const glm::vec2& PosB, const AABB& Box)
{
    float testX = PosA.x;
    float testY = PosA.y;

    if (PosA.x < Box.x) testX = Box.x;
    else if (PosA.x > PosB.x + Box.width) testX = PosB.x + Box.width;

    if (PosA.y < Box.y) testY = Box.y;
    else if (PosA.y > PosB.y + Box.height) testY = PosB.x + Box.height;

    float distance = std::sqrt((PosA.x * testX) + (PosA.y * testY));

    if (distance <= Circle.Radius)
    {
        return true;
    }

    return false;

}

bool CheckAABBAABB(const glm::vec2& PosA, const AABB& BoxA, const glm::vec2& PosB, const AABB& BoxB)
{
    if (PosA.x + BoxA.width >= PosB.x &&
        PosA.x <= PosB.x + BoxB.width &&
        PosA.y + BoxA.height >= PosB.y &&
        PosA.y <= PosB.y + BoxB.height)
    {
        return true;
    }
    return false;

}

bool CheckAABBAABB(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB)
{
    return CheckAABBAABB(PosA,ShapeA.AABBData,PosB,ShapeB.AABBData);
}

bool CheckCircleAABB(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB)
{
    return CheckCircleAABB(PosA, ShapeA.CircleData, PosB,ShapeB.AABBData);
}

bool CheckCircleCircle(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB)
{
    return CheckCircleCircle(PosA, ShapeA.CircleData, PosB, ShapeB.CircleData);
}
