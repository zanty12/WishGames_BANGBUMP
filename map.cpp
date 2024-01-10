/**
 * @file map.cpp
 * @brief Map�N���X���������܂��B
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
 * @brief �w�肵���ʒu�ɃZ����z�u���܂��B
 * @param x �Z����z�u����X���W�B
 * @param y �Z����z�u����Y���W�B
 * @param MAP_READ �Z���̎�ނ�\�������l�B
 *
 * ���̃��\�b�h�́A�w�肵���ʒu���}�b�v�͈͓̔��ɂ���ꍇ�ɂ̂݃Z����z�u���܂��B
 * MAP_READ�̒l�ɉ����āA�قȂ��ނ̃Z�����z�u����܂��B
 * �Ⴆ�΁AMAP_READ_WALL�̏ꍇ��Wall�AMAP_READ_FLOOR�̏ꍇ��Floor���z�u����܂��B
 * MAP_READ_ORB_SMALL�AMAP_READ_ORB_MID�AMAP_READ_ORB_BIG�̏ꍇ�́A���ꂼ��قȂ�傫����SkillOrbAttribute���z�u����܂��B
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
  * @brief �}�b�v���̂��ׂẴZ�����X�V���܂��B
  *
  * ���̃��\�b�h�́A�}�b�v���̊e�Z����Update���\�b�h���Ăяo���܂��B
  */
void Map::Update()
{
    for (int i = 0; i < width_ * height_; i++)
    {
        if (map_[i] != nullptr)
            map_[i]->Update();
    }
}

