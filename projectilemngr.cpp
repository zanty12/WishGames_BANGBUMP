#include "projectilemngr.h"

void ProjectileMngr::Update()
{
    for(auto it = projectiles_.begin(); it != projectiles_.end();)
    {
        if((*it)->GetDiscard())
        {
            delete *it;
            it = projectiles_.erase(it);
        }
        else
        {
            (*it)->Update();
            ++it;
        }
    }
}


