#include <memory>
#include "World.h"
#include "PhysObject.h"
int main()
{
    auto App = std::make_unique<World>();
    App->Init();
  

    while (!App->ShouldClose())
    {
        App->Tick();
        while (App->ShouldFixedTick())
        {
            App->FixedTick();
        }
        App->Draw();
    }
    App->Exit();

    return 0;
}