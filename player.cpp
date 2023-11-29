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
	using namespace PHYSICS;

	Vertex4 square(Vector2(-1, 1), Vector2(1, 1), Vector2(1, -1), Vector2(-1, -1));
	Vertex1 point(Vector2(0, 0), 100);
	Collider2D::Touch(point, square);



}

//================================================================================
// ↓パブリック関数↓
//================================================================================

bool Player::CollisionBB(Vector2 others_pos, float others_size)
{
	using namespace PHYSICS;

	Vector2 left_head = Vector2(GetPos().x - scale_.x / 2, GetPos().y + scale_.y / 2);
	Vector2 right_head = Vector2(GetPos().x + scale_.x / 2, GetPos().y + scale_.y / 2);
	Vector2 left_bottom= Vector2(GetPos().x - scale_.x / 2, GetPos().y - scale_.y / 2);
	Vector2 right_bottom = Vector2(GetPos().x + scale_.x / 2, GetPos().y - scale_.y / 2);

	Vertex4 player_box(left_head, right_bottom, left_bottom, right_bottom);

	Vertex1 others_point(others_pos, others_size);

	if (Collider2D::Touch(others_point, player_box))
	{
		return true;
	}

	return false;
}

void Player::CollisionMap(void)
{
	Vector2 top(GetPos().x, GetPos().y + size_);
	Vector2 under(GetPos().x, GetPos().y - size_);
	Vector2 right(GetPos().x + size_, GetPos().y);
	Vector2 left(GetPos().x - size_, GetPos().y);

	Cell* map_cell;

	//頭の判定
	map_cell = map_mangr_->GetMap()->GetCell(top.x, top.y);



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
