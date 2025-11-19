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
    DepenMap[ShapeType::CIRCLE | ShapeType::AABB] = DepenetrateCircleAABB;
    DepenMap[ShapeType::AABB | ShapeType::AABB] = DepenetrateAABBAABB;


    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    
    std::shared_ptr<PhysObject> Object1 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object2 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object3 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object4 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object5 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object6 = std::make_shared<PhysObject>();
   
    Object1->Collider.Type = ShapeType::AABB;
    Object1->Collider.AABBData.x = 0;
    Object1->Collider.AABBData.y = 0;
    Object1->Collider.AABBData.width = 10;
    Object1->Collider.AABBData.height = 10;
    Object1->Position = { 10,10 };
    Object1->AddForce({ 500,0 });

    Object2->Collider.Type = ShapeType::AABB;
    Object2->Collider.AABBData.height = 10;
    Object2->Collider.AABBData.width = 10;
    Object2->Collider.AABBData.x = 0;
    Object2->Collider.AABBData.y = 0;
    Object2->Position = { 40,10 };

    Object3->Collider.Type = ShapeType::CIRCLE;
    Object3->Collider.CircleData.Radius = 10;
    Object3->Position = { 10,50 };
    Object3->AddForce({ 500,0 });

    Object4->Collider.Type = ShapeType::CIRCLE;
    Object4->Collider.CircleData.Radius = 10;
    Object4->Position = { 40,50 };


    Object5->Collider.Type = ShapeType::CIRCLE;
    Object5->Collider.CircleData.Radius = 10;
    Object5->Position = { 10,80 };
    Object5->AddForce({ 500,0 });

    Object6->Collider.Type = ShapeType::AABB;
    Object6->Collider.AABBData.x = 0;
    Object6->Collider.AABBData.y = 0;
    Object6->Collider.AABBData.width = 10;
    Object6->Collider.AABBData.height = 10;
    Object6->Position = { 40,80 };
    
    
    Instantiate(Object1);
    Instantiate(Object2);
    Instantiate(Object3);
    Instantiate(Object4);
    Instantiate(Object5);
    Instantiate(Object6);


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
               
                float Pen = 0.0f;
                glm::vec2 Normal = DepenMap[result](i->Position, i->Collider,
                    j->Position, j->Collider, Pen);

                ResolvePhysObjects(*i,*j,1.0f,Normal,Pen);
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

