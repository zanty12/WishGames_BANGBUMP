#pragma once
#include "map.h"
#include "mapmngr.h"
class MapMngr
{
private:
	Map* map_;
public:
	MapMngr() = delete;
	MapMngr(const char* filename);
	~MapMngr()
	{
		delete map_;
	};
private:
	void Read(const char* filename);
};
