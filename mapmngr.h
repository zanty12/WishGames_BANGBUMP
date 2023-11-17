#pragma once
#include "map.h"
#include "mapmngr.h"
class MapMngr
{
private:
	Map* map_;
	Vector2 spawn_;
public:
	MapMngr() = delete;
	MapMngr(const char* file_name);
	~MapMngr()
	{
		delete map_;
	};
private:
	bool Read(const char* file_name);
};
