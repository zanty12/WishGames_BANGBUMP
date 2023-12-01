#pragma once
#include "camera.h"
#include "map.h"
#include "mapmngr.h"
#include "enemymngr.h"

#include "game.h"

class EnemyMngr;
class Game;
class Camera;

class MapMngr
{
private:
	Map* map_;
	Vector2 spawn_;
	EnemyMngr* enemy_mngr_;

	Game* game_; //‚ ‚½‚è”»’è—p
public:
	MapMngr() = delete;
	MapMngr(const char* file_name,Game* game);
	~MapMngr()
	{
		delete map_;
	};
	Map* GetMap() const
	{
		return map_;
	}
	void Update() const;
	void Draw(Camera* camera) const;

	void DebugMenu();

	Vector2 GetPlayerSpawn() const
	{
		return spawn_;
	}

	EnemyMngr* GetEnemyMngr() const
	{
		return enemy_mngr_;
	}

	Game* GetGame() const
	{
		return game_;
	}
private:
	bool Read(const char* file_name);
	void ArrangeBlkTex() const;
};
