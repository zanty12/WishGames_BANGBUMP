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
#include "wall.h"
#include "mapenum.h"
#include "spike.h"
#include "area_capture.h"
#include "camera.h"

/**
 * @brief 指定した位置にセルを配置します。
 * @param x セルを配置するX座標。
 * @param y セルを配置するY座標。
 * @param MAP_READ セルの種類を表す整数値。
 *
 * このメソッドは、指定した位置がマップの範囲内にある場合にのみセルを配置します。
 * MAP_READの値に応じて、異なる種類のセルが配置されます。
 * 例えば、MAP_READ_WALLの場合はWall、MAP_READ_FLOORの場合はFloorが配置されます。
 * MAP_READ_ORB_SMALL、MAP_READ_ORB_MID、MAP_READ_ORB_BIGの場合は、それぞれ異なる大きさのSkillOrbAttributeが配置されます。
 */
void Map::PutCell(int x, int y, int MAP_READ) const
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return;
    switch (MAP_READ)
    {
    case MAP_READ_NONE:
        break;
    case MAP_READ_WALL:
        {
            Wall* wall = new Wall(x, y);
            map_[y * width_ + x] = wall;
            break;
        }
    case MAP_READ_FLOOR:
        {
            Floor* floor = new Floor(x, y);
            map_[y * width_ + x] = floor;
            break;
        }
    case MAP_READ_PENFLOOR:
        {
            PenFloor* penfloor = new PenFloor(x, y);
            map_[y * width_ + x] = penfloor;
            break;
        }
    case MAP_READ_BLOCK:
        {
            Block* block = new Block(x, y);
            map_[y * width_ + x] = block;
            break;
        }

    case MAP_READ_CLOUD:
        {
            Cloud * cloud = new Cloud(x, y);
            map_[y * width_ + x] = cloud;
            break;
        }

    case MAP_READ_ORB_SMALL:
        {
            GameBase::GetSkillOrbMngr()->Pop(x, y, SKILLORB_SIZE_TYPE_SMALL);
            break;
        }

    case MAP_READ_ORB_MID:
        {
            GameBase::GetSkillOrbMngr()->Pop(x, y, SKILLORB_SIZE_TYPE_MID);
            break;
        }

    case MAP_READ_ORB_BIG:
        {
            GameBase::GetSkillOrbMngr()->Pop(x, y, SKILLORB_SIZE_TYPE_BIG);
            break;
        }

    case MAP_READ_SPIKE_UP:
        {
            Spike * spike = new Spike(x, y, MAP_READ_SPIKE_UP);
            map_[y * width_ + x] = spike;
            break;
        }

    case MAP_READ_SPIKE_DOWN:
        {
            Spike* spike = new Spike(x, y, MAP_READ_SPIKE_DOWN);
            map_[y * width_ + x] = spike;
            break;
        }

    case MAP_READ_SPIKE_LEFT:
        {
            Spike* spike = new Spike(x, y, MAP_READ_SPIKE_LEFT);
            map_[y * width_ + x] = spike;
            break;
        }

    case MAP_READ_SPIKE_RIGHT:
        {
            Spike* spike = new Spike(x, y, MAP_READ_SPIKE_RIGHT);
            map_[y * width_ + x] = spike;
            break;
        }

    case MAP_READ_MULTI_AREA_CAPTURE:
        {
            AreaCapture *spike = new AreaCapture(x, y);
            map_[y * width_ + x] = spike;
            break;
        }
    default:
        break;
    }
}

/**
  * @brief マップ内のすべてのセルを更新します。
  *
  * このメソッドは、マップ内の各セルでUpdateメソッドを呼び出します。
  */
void Map::Update()
{
    for (int i = 0; i < width_ * height_; i++)
    {
        if (map_[i] != nullptr)
            map_[i]->Update();
    }
}

