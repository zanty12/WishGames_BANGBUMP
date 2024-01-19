#include "gamebase.h"

Renderer *GameBase::renderer_ = new Renderer();
CollMngr *GameBase::coll_mngr_ = new CollMngr();
ProjectileMngr *GameBase::projectile_mngr_ = new ProjectileMngr();
SkillOrbMngr *GameBase::orb_mngr_ = new SkillOrbMngr();
std::list<Player *> GameBase::players_ = std::list<Player *>();

GameBase::~GameBase() {
    if (mapmngr_) 
    {
        delete mapmngr_;
        mapmngr_ = nullptr;
    }
    if (camera_) 
    {
        delete camera_;
        camera_ = nullptr;
    }
    delete renderer_;
    delete coll_mngr_;
    delete projectile_mngr_;
    delete orb_mngr_;

    if (!players_.empty())
    {
        for (auto player : players_) {
            delete player;
        }
        players_.clear();
    }

}

bool GameBase::UpdateLoD(const Vector2 pos)
{
    bool ret = false;
    // get distance from the closest player
    float dist = 0.0f;
    for (auto player : players_) {
        float d = Vector2::Distance(pos, player->GetPos());
        if (dist == 0.0f || d < dist) {
            dist = d;
        }
    }
    if(dist > LoD_threshold_) {
        ret = true;
    }
    return ret;
}



