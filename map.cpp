/**
 * @file map.cpp
 * @brief Mapクラスを実装します。
 * @author Cheung To Hung
 * @date 2023/11/22
 */

#include "map.h"
#include "block.h"
#include "cloud.h"
#include "floor.h"
#include "penfloor.h"
#include "skillorb.h"
#include "thundercloud.h"
#include "wall.h"
#include "mapenum.h"

/**
 * @brief 指定した位置にセルを配置します。
 * @param x セルを配置するX座標。
 * @param y セルを配置するY座標。
 * @param MAP_READ セルの種類を表す整数値。
 *
 * このメソッドは、指定した位置がマップの範囲内にある場合にのみセルを配置します。
 * MAP_READの値に応じて、異なる種類のセルが配置されます。
 * 例えば、MAP_READ_WALLの場合はWall、MAP_READ_FLOORの場合はFloorが配置されます。
 * MAP_READ_ORB_SMALL、MAP_READ_ORB_MID、MAP_READ_ORB_BIGの場合は、それぞれ異なる大きさのSkillOrbが配置されます。
 */
void Map::PutCell(int x, int y, int MAP_READ) const
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