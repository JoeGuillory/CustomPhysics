#include "World.h"
#include "raylib.h"
#include "PhysObject.h"
World::World() : AccumulatedFixedTime(0), TargetFixedStep(1.0f / 30.0f), bUseGravity(true), GravityScale(.5)
{


}

void World::Init()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    std::shared_ptr<PhysObject> Object1 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object2 = std::make_shared<PhysObject>();
    std::shared_ptr<PhysObject> Object3 = std::make_shared<PhysObject>();
    
    Object1->Position = { 5,5 };
    Object2->Position = { 2,5 };
    Object3->Position = { 3,9 };
    Object1->Collider.Type = ShapeType::NONE;
    Object2->Collider.Type = ShapeType::NONE;
    Object3->Collider.Type = ShapeType::NONE;
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

