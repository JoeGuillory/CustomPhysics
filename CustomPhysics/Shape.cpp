#include "Shape.h"
#include <cmath>
#include "glm/glm.hpp"


bool CheckCircleCircle(const glm::vec2& PosA, const Circle& CircleA, const glm::vec2& PosB, const Circle& CircleB)
{
    
    float distX = PosA.x - PosB.x;
    float distY = PosA.y - PosB.y;
    float distance = sqrt((distX * distX) + (distY * distY));
    if (distance <= CircleA.Radius + CircleB.Radius)
    {
        return true;
    }
    return false;
}

bool CheckCircleAABB(const glm::vec2& PosA, const Circle& Circle, const glm::vec2& PosB, const AABB& Box)
{
    glm::vec2 test;
    test = PosA;

    if (PosA.x < PosB.x + Box.x) test.x = PosB.x + Box.x;
    else if (PosA.x > PosB.x + Box.x + Box.width) test.x = PosB.x + Box.x + Box.width;

    if (PosA.y < PosB.y + Box.y) test.y = PosB.y + Box.y;
    else if (PosA.y > PosB.y + Box.y + Box.height) test.y = PosB.y + Box.y + Box.height;

    glm::vec2 dist = PosA - test;
    float distance = glm::sqrt((dist.x * dist.x) + (dist.y * dist.y));

    return distance <= Circle.Radius;

}

bool CheckAABBAABB(const glm::vec2& PosA, const AABB& BoxA, const glm::vec2& PosB, const AABB& BoxB)
{
    if (PosA.x  + BoxA.x + BoxA.width >= PosB.x + BoxB.x &&
        PosA.x + BoxA.x <= PosB.x + BoxB.x + BoxB.width &&
        PosA.y + BoxA.y + BoxA.height >= PosB.y + BoxB.y &&
        PosA.y + BoxA.y <= PosB.y + BoxA.y + BoxB.height)
    {
        return true;
    }
    return false;

}

glm::vec2 DepenetrateCircleCircle(const glm::vec2& PosA, const Circle& CircleA, const glm::vec2& PosB, const Circle& CircleB, float& Pen)
{
    // get the distance between the two circles
    float Dist = glm::length(PosB - PosA);
    // add up the sum of the two radii
    float Radii = CircleA.Radius + CircleB.Radius;

    // find the difference and write it to the referenced variable
    Pen = Radii - Dist;

    // return the direction to correct along
    return glm::normalize(PosB - PosA);
}

glm::vec2 DepenetrateCircleAABB(const glm::vec2& PosA, const Circle& Circle, const glm::vec2& PosB, const AABB& Box, float& Pen)
{

    glm::vec2 test;
    test = PosA;
    if (PosA.x < PosB.x + Box.x) test.x = PosB.x + Box.x;
    else if (PosA.x > PosB.x + Box.x + Box.width) test.x = PosB.x + Box.x + Box.width;

    if (PosA.y < PosB.y + Box.y) test.y = PosB.y + Box.y;
    else if (PosA.y > PosB.y + Box.y + Box.height) test.y = PosB.y + Box.y + Box.height;
    
    glm::vec2 dist = PosA - test;

    float distance = glm::sqrt((dist.x * dist.x) + (dist.y * dist.y));

    Pen = Circle.Radius - distance;

    return glm::normalize(test - PosA);

}

glm::vec2 DepenetrateAABBAABB(const glm::vec2& PosA, const AABB& BoxA, const glm::vec2& PosB, const AABB& BoxB, float& Pen)
{
    
    glm::vec2 centerA = PosA + glm::vec2(BoxA.x, BoxA.y);
    glm::vec2 centerB = PosB + glm::vec2(BoxB.x, BoxB.y);

   
    glm::vec2 halfA(BoxA.width * 0.5f, BoxA.height * 0.5f);
    glm::vec2 halfB(BoxB.width * 0.5f, BoxB.height * 0.5f);

    
    glm::vec2 delta = centerB - centerA;

    
    float overlapX = (halfA.x + halfB.x) - std::abs(delta.x);
    float overlapY = (halfA.y + halfB.y) - std::abs(delta.y);

  
    if (overlapX <= 0.0f || overlapY <= 0.0f)
    {
        Pen = 0.0f;
        return glm::vec2(0.0f);
    }

    glm::vec2 normal(0.0f);

    
    if (overlapX < overlapY)
    {
        Pen = overlapX;
        normal.x = (delta.x > 0.0f) ? 1.0f : -1.0f;
    }
    else
    {
        Pen = overlapY;
        normal.y = (delta.y > 0.0f) ? 1.0f : -1.0f;
    }

    return normal;
}

glm::vec2 DepenetrateCircleCircle(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB, float& Pen)
{
    return DepenetrateCircleCircle(PosA, ShapeA.CircleData, PosB, ShapeB.CircleData, Pen);
}

glm::vec2 DepenetrateCircleAABB(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB, float& Pen)
{
    return DepenetrateCircleAABB(PosA, ShapeA.CircleData, PosB, ShapeB.AABBData, Pen);
}

glm::vec2 DepenetrateAABBAABB(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB, float& Pen)
{
    return DepenetrateAABBAABB(PosA, ShapeA.AABBData, PosB, ShapeB.AABBData, Pen);
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
