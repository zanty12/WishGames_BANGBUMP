#include "gamebase.h"

Renderer *GameBase::renderer_ = new Renderer();
CollMngr *GameBase::coll_mngr_ = new CollMngr();
ProjectileMngr *GameBase::projectile_mngr_ = new ProjectileMngr();
SkillOrbMngr *GameBase::orb_mngr_ = new SkillOrbMngr();

GameBase::~GameBase() {
    if (mapmngr_) delete mapmngr_;
    if (camera_) delete camera_;
    delete renderer_;
    delete coll_mngr_;
    delete projectile_mngr_;
    delete orb_mngr_;

    for (auto player : players_) {
        delete player;
    }
}
