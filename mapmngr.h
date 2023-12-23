#pragma once
#include "camera.h"
#include "map.h"
#include "mapmngr.h"
#include "enemymngr.h"

#include "gamebase.h"

class EnemyMngr;
class Game;
class Camera;
class GameBase;

class MapMngr
{
private:
	Map* map_;
	Vector2 spawn_;
	EnemyMngr* enemy_mngr_;

	GameBase* game_; //‚ ‚½‚è”»’è—p
public:
	MapMngr() = delete;
	MapMngr(const char* file_name,GameBase* game);
	~MapMngr()
	{
		delete map_;
	};
	Map* GetMap() const
	{
		return map_;
	}
	void Update() const;

	void DebugMenu();

	Vector2 GetPlayerSpawn() const
	{
		return spawn_;
	}

	EnemyMngr* GetEnemyMngr() const
	{
		return enemy_mngr_;
	}

	GameBase* GetGame() const
	{
		return game_;
	}
private:
	bool Read(const char* file_name);
	void ArrangeBlkTex() const;
};
