#include "map.h"

#include "block.h"
#include "cloud.h"
#include "floor.h"
#include "penfloor.h"
#include "skillorb.h"
#include "thundercloud.h"
#include "wall.h"

void Map::PutCell(int x, int y, int cell_type)
{
	if (x < 0 || x >= width_ || y < 0 || y >= height_)
		return;
	switch (cell_type)
	{
	case CELL_TYPE_WALL:
		map_[y * width_ + x] = new Wall(x, y);
		break;
	case CELL_TYPE_FLOOR:
		map_[y * width_ + x] = new Floor(x, y);
		break;
	case CELL_TYPE_PENFLOOR:
		map_[y * width_ + x] = new PenFloor(x, y);
		break;
	case CELL_TYPE_BLOCK:
		map_[y * width_ + x] = new Block(x, y);
		break;
	case CELL_TYPE_CLOUD:
		map_[y * width_ + x] = new Cloud(x, y);
		break;
	case CELL_TYPE_THUNDERCLOUD:
		map_[y * width_ + x] = new ThunderCloud(x, y);
		break;
	case CELL_TYPE_ORB_SMALL:
		map_[y * width_ + x] = new SkillOrb(x, y, CELL_TYPE_ORB_SMALL);
		break;
	case CELL_TYPE_ORB_MID:
		map_[y * width_ + x] = new SkillOrb(x, y, CELL_TYPE_ORB_MID);
		break;
	case CELL_TYPE_ORB_BIG:
		map_[y * width_ + x] = new SkillOrb(x, y, CELL_TYPE_ORB_BIG);
		break;
	default:
		break;
	}
}