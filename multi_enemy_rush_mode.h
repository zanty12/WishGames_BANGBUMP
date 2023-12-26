#pragma once
#include "multi_mode.h"
#include "lib/collider2d.h"
#include "time.h"

class MultiPlayEnemyRushMode : public MultiPlayServerSide {


public:
	MultiPlayEnemyRushMode(MapMngr *map) : MultiPlayServerSide(map) { }

	void GameUpdate(std::list<CLIENT_DATA_SERVER_SIDE>& clients) override {
		
	}



};
