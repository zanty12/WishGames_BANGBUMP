#include "projectilemngr.h"

void ProjectileMngr::Update()
{
    for(auto projectile : projectiles_)
    {
        if(projectile == nullptr)
            continue;

        projectile->Update();

        if(projectile->GetDiscard())
        {
            delete projectile;
            projectile = nullptr;
        }
    }
}