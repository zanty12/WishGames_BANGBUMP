#pragma once
#include "map.h"
#include "mapmngr.h"
#include "enemymngr.h"
class MapMngr
{
private:
	Map* map_;
	Vector2 spawn_;
	EnemyMngr* enemy_mngr_;
public:
	MapMngr() = delete;
	MapMngr(const char* file_name);
	~MapMngr()
	{
		delete map_;
	};
	Map* GetMap() const
	{
		return map_;
	}
	void Update() const
	{
		map_->Update();
	}
	void Draw() const
	{
		map_->Draw();
	}
	void DebugMenu();

	Vector2 GetPlayerSpawn() const
	{
		return spawn_;
	}
private:
	bool Read(const char* file_name);
	void ArrangeBlkTex() const;
};
