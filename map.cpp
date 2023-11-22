#include "map.h"

#include "block.h"
#include "cloud.h"
#include "floor.h"
#include "penfloor.h"
#include "skillorb.h"
#include "thundercloud.h"
#include "wall.h"
#include "mapenum.h"

void Map::PutCell(int x, int y, int MAP_READ)
{
	if (x < 0 || x >= width_ || y < 0 || y >= height_)
		return;
	switch (MAP_READ)
	{
	case MAP_READ_WALL:
		map_[y * width_ + x] = new Wall(x, y);
		break;
	case MAP_READ_FLOOR:
		map_[y * width_ + x] = new Floor(x, y);
		break;
	case MAP_READ_PENFLOOR:
		map_[y * width_ + x] = new PenFloor(x, y);
		break;
	case MAP_READ_BLOCK:
		map_[y * width_ + x] = new Block(x, y);
		break;
	case MAP_READ_CLOUD:
		map_[y * width_ + x] = new Cloud(x, y);
		break;
	case MAP_READ_THUNDERCLOUD:
		map_[y * width_ + x] = new ThunderCloud(x, y);
		break;
	case MAP_READ_ORB_SMALL:
		map_[y * width_ + x] = new SkillOrb(x, y, MAP_READ_ORB_SMALL);
		break;
	case MAP_READ_ORB_MID:
		map_[y * width_ + x] = new SkillOrb(x, y, MAP_READ_ORB_MID);
		break;
	case MAP_READ_ORB_BIG:
		map_[y * width_ + x] = new SkillOrb(x, y, MAP_READ_ORB_BIG);
		break;
	default:
		break;
	}
}