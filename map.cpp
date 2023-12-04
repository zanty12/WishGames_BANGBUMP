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
#include "thundercloud.h"
#include "wall.h"
#include "mapenum.h"
#include "spike.h"
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
 * MAP_READ_ORB_SMALL�AMAP_READ_ORB_MID�AMAP_READ_ORB_BIG�̏ꍇ�́A���ꂼ��قȂ�傫����SkillOrb���z�u����܂��B
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
        map_[y * width_ + x] = new Wall(x, y);
        break;
    case MAP_READ_FLOOR:
        {
        }
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
    case MAP_READ_SPIKE_UP:
        map_[y * width_ + x] = new Spike(x, y, MAP_READ_SPIKE_UP);
        break;
    case MAP_READ_SPIKE_DOWN:
        map_[y * width_ + x] = new Spike(x, y, MAP_READ_SPIKE_DOWN);
        break;
    case MAP_READ_SPIKE_LEFT:
        map_[y * width_ + x] = new Spike(x, y, MAP_READ_SPIKE_LEFT);
        break;
    case MAP_READ_SPIKE_RIGHT:
        map_[y * width_ + x] = new Spike(x, y, MAP_READ_SPIKE_RIGHT);
        break;
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

void Map::Draw(Camera* camera) const
{
    for (int i = 0; i < width_ * height_; i++)
    {
        if (map_[i] != nullptr && camera->InCamera(map_[i]))
        {
            map_[i]->Draw(camera->GetCameraOffset());
        }
    }
}
