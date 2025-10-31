#include "World.h"
#include "raylib.h"
#include "PhysObject.h"
#include "EnumUtils.h"
#include <iostream>

using CollisionFunc = bool(*)(const glm::vec2&, const Shape&, const glm::vec2&, const Shape&);
using CollisionMap = std::unordered_map<ShapeType, CollisionFunc>;
CollisionMap map;
World::World() : AccumulatedFixedTime(0), TargetFixedStep(1.0f / 30.0f), bUseGravity(true), GravityScale(.5)
{
   
    

}

void World::Init()
{

    map[ShapeType::CIRCLE | ShapeType::CIRCLE] = CheckCircleCircle;
    map[ShapeType::CIRCLE | ShapeType::AABB] = CheckCircleAABB;
    map[ShapeType::AABB | ShapeType::AABB] = CheckAABBAABB;


    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    
    std::shared_ptr<PhysObject> Object1 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object2 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object3 = std::make_shared<PhysObject>();
    
    Object1->Position = { 10,5 };
    Object2->Position = { 20,4 };
    Object3->Position = { 30,9 };
    Object1->Collider.Type = ShapeType::CIRCLE;
    Object2->Collider.Type = ShapeType::CIRCLE;
    Object3->Collider.Type = ShapeType::CIRCLE;

    Object1->Collider.CircleData.Radius = 5;
    Object2->Collider.CircleData.Radius = 10;
    Object3->Collider.CircleData.Radius = 10;
    Object1->AddForce({ 20,0 });
    Object2->AddForce({ 20,0 });
    Instantiate(Object1);
    Instantiate(Object2);
    Instantiate(Object3);


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
            i->AddAccel(glm::vec2{ 0, 9.81f } *GravityScale);
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
                std::cout << "there was a hit" << std::endl;
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

