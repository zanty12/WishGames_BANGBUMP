#pragma once
#include "movableobj.h"

class ProjectileMngr
{
private:
    std::list<MovableObj*> projectiles_;
public:
    ProjectileMngr() = default;
    ~ProjectileMngr()
    {
        for(MovableObj* projectile : projectiles_)
        {
            delete projectile;
        }
        projectiles_.clear();
    }
    void Update();
    void Add(MovableObj* projectile)
    {
        if(projectile != nullptr)
            projectiles_.push_back(projectile);
    }
    std::list<MovableObj*> GetProjectiles() const { return projectiles_; }
};
