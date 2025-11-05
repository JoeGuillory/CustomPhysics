#include "World.h"
#include "raylib.h"
#include "PhysObject.h"
#include "EnumUtils.h"
#include <iostream>

using CollisionFunc = bool(*)(const glm::vec2&, const Shape&, const glm::vec2&, const Shape&);
using CollisionMap = std::unordered_map<ShapeType, CollisionFunc>;
using DepenetrationFunc = glm::vec2(*)(const glm::vec2& PosA, const Shape& ShapeA, const glm::vec2& PosB, const Shape& ShapeB, float& Pen);
using DepenetrationMap = std::unordered_map <ShapeType, DepenetrationFunc>;
CollisionMap map;
DepenetrationMap DepenMap;
World::World() : AccumulatedFixedTime(0), TargetFixedStep(1.0f / 30.0f), bUseGravity(true), GravityScale(.5)
{
   
    

}

void World::Init()
{

    map[ShapeType::CIRCLE | ShapeType::CIRCLE] = CheckCircleCircle;
    map[ShapeType::CIRCLE | ShapeType::AABB] = CheckCircleAABB;
    map[ShapeType::AABB | ShapeType::AABB] = CheckAABBAABB;

    DepenMap[ShapeType::CIRCLE | ShapeType::CIRCLE] = DepenetrateCircleCircle;


    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    
    std::shared_ptr<PhysObject> Object1 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object2 = std::make_shared<PhysObject>();
   
    Object1->Collider.Type = ShapeType::CIRCLE;
    Object1->Collider.CircleData.Radius = 6.0f;
    Object1->Position = { 10,10 };
   Object1->AddForce({ 500,0 });

    Object2->Collider.Type = ShapeType::AABB;
    Object2->Collider.AABBData.height = 10;
    Object2->Collider.AABBData.width = 10;
    Object2->Collider.AABBData.x = 0;
    Object2->Collider.AABBData.y = 0;
    Object2->Position = { 40,10 };

    
    Instantiate(Object1);
    Instantiate(Object2);
 


    SetTargetFPS(60);
    OnInit();
}

void World::Tick()
{
    AccumulatedFixedTime += GetFrameTime();
    OnTick();
}

void World::FixedTick()
{
    AccumulatedFixedTime -= TargetFixedStep;

    // integrate forces and velocity for all objects
    for (auto i : PhysObjects)
    {
        // gravity, if applicable
        if (bUseGravity && i->bUseGravity)
        {
            i->AddAccel(glm::vec2{ 0, 9.81f } * GravityScale);
        }

        // integrate velocity into position
        i->TickPhys(TargetFixedStep);

    }

    for (auto& i : PhysObjects)
    {
        for (auto& j : PhysObjects)
        {
            if (i == j)
                continue;
            ShapeType result = i->Collider.Type | j->Collider.Type;
            bool collisionResult = false;
            if (i->Collider.Type > j->Collider.Type)
            {
                collisionResult = map[result](j->Position, j->Collider, i->Position, i->Collider);
            }
            else
            {
                collisionResult = map[result](i->Position, i->Collider, j->Position, j->Collider);
            }

            if (collisionResult)
            {
                std::cout << "Collision Works" << std::endl;
               /* float Pen = 0.0f;
                glm::vec2 Normal = DepenMap[result](i->Position, i->Collider,
                    j->Position, j->Collider, Pen);

                ResolvePhysObjects(*i,*j,1.0f,Normal,Pen);*/
            }
        }
    }

    OnFixedTick();
}

void World::Draw() const
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (auto object : PhysObjects)
    {
        object->Draw();
    }
    OnDraw();
    EndDrawing();
}

void World::Exit()
{
    OnExit();
    CloseWindow();
}

bool World::ShouldClose() const
{
    return WindowShouldClose();
}

bool World::ShouldFixedTick() const
{
    return AccumulatedFixedTime >= TargetFixedStep;
}

void World::Instantiate(std::shared_ptr<PhysObject> Object)
{
    PhysObjects.push_back(Object);
}

