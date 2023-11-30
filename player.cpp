//--------------------------------------------------------------------------------
// 
// プレイヤー[player.cpp]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/17
// 最終更新日	2023/11/20
// 
//--------------------------------------------------------------------------------

#include "player.h"
#include "lib/collider2d.h"

bool Player::UseSkillPoint(void)
{
	if (skillpt_ == SKILL_GAUGE_MAX_)
	{
		skillpt_ = 0;
		return true;
	}

	return false;
}

void Player::Update(void)
{

	if (move_attribute_ != nullptr)
	{
		SetVel(move_attribute_->Move());
	}
	else
	{//何も操作しなければ落ちる
		if (GetVel().y <= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y - 0.05f));
	}

	if (attack_attribute_ != nullptr)
	{
		attack_attribute_->Action();
	}

	AddVel(GetVel());
	UpdateDir();
	CollisionMap();
	using namespace PHYSICS;

	Vertex4 square(Vector2(-1, 1), Vector2(1, 1), Vector2(1, -1), Vector2(-1, -1));
	Vertex1 point(Vector2(0, 0), 100);
	Collider2D::Touch(point, square);



}

//================================================================================
// ↓パブリック関数↓
//================================================================================


void Player::CollisionMap(void)
{
	Map* map = GetMapMngr()->GetMap();
	Cell* cells[4] = {nullptr, nullptr, nullptr, nullptr};
	int idx = std::floor(GetPos().x / size_);
	int idy = std::floor(GetPos().y / size_);
	cells[0] = map->GetCell(idx, idy + 1);
	cells[1] = map->GetCell(idx, idy - 1);
	cells[2] = map->GetCell(idx - 1, idy);
	cells[3] = map->GetCell(idx + 1, idy);
	for (int i = 0; i < 4; i++)
	{
		if (cells[i] == nullptr)
			continue;
		if (Collision(cells[i]))
			MapCellInteract(cells[i]);
	}
}
/*
MAP_READ_NONE, ///< 空のセルを表します。
MAP_READ_WALL, ///< 壁セルを表します。
MAP_READ_FLOOR, ///< 床セルを表します。
MAP_READ_PENFLOOR, ///< 貫通可能な床セルを表します。
MAP_READ_CLOUD, ///< 雲セルを表します。
MAP_READ_THUNDERCLOUD, ///< 雷雲セルを表します。
MAP_READ_BLOCK, ///< ブロックセルを表します。
MAP_READ_ORB_SMALL, ///< 小スキル玉セルを表します。
MAP_READ_ORB_MID, ///< 中スキル玉セルを表します。
MAP_READ_ORB_BIG, ///< 大スキル玉セルを表します。
MAP_READ_SPIKE_LEFT, ///< 左向きのトゲセルを表します。
MAP_READ_SPIKE_RIGHT, ///< 右向きのトゲセルを表します。
MAP_READ_SPIKE_UP, ///< 上向きのトゲセルを表します。
MAP_READ_SPIKE_DOWN, ///< 下向きのトゲセルを表します。
*/
