//--------------------------------------------------------------------------------
// 
// プレイヤー[player.cpp]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/17
// 最終更新日	2023/12/15
// 
//--------------------------------------------------------------------------------

#include "player.h"
#include "lib/collider2d.h"
#include "spike.h"
#include "xinput.h"


#define LVUP_EXP_2	(2)
#define LVUP_EXP_3	(5)
#define LVUP_EXP_4	(9)
#define LVUP_EXP_5	(14)
#define LVUP_EXP_6	(20)
#define LVUP_EXP_7	(29)
#define LVUP_EXP_8	(41)
#define LVUP_EXP_9	(55)
#define LVUP_EXP_10	(71)

bool Player::LvUp(int get_skill_pt)
{
	skillpt_ += get_skill_pt;

	//なんか効率のいい方法ない？
	if (skillpt_ >= LVUP_EXP_2) {
		lv_ = 2;
	}
	if (skillpt_ >= LVUP_EXP_3) {
		lv_ = 3;
	}
	if (skillpt_ >= LVUP_EXP_4) {
		lv_ = 4;
	}
	if (skillpt_ >= LVUP_EXP_5) {
		lv_ = 5;
	}

	return false;
}

void Player::Update(void)
{

	//HPが0になったらリザルトに移る
	if (hp_ <= 0)
	{
		change_scene_ = true;
	}

	//5フレーム中に何も操作していなかったら落ちる動作に移行する
	if (Input::GetStickLeft(0).x == 0.0f && Input::GetStickLeft(0).y == 0.0f)
	{
		not_stick_working_++;
	}
	else
	{
		not_stick_working_ = 0;
	}

	bool affected_gravity = false;	//重力を受けたかどうか

	if (move_attribute_ != nullptr && clash_spike_ == 0)
	{
		(void)move_attribute_->Move();
		move_attribute_->Gravity();

	}
	else if (clash_spike_ == 0)
	{//何も属性がなければ落ちる
		if (GetVel().y >= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y - 0.05f));
	}


	if (attack_attribute_ != nullptr)
	{
		attack_attribute_->Action();
	}

	UpdateDir();

	AddVel(GetVel());

	CollisionMap();

	CollisionSpike();

	//上に上がっている
	if (GetVel().y > 0.0f)
	{
		player_state_ = MOVE_UP;
	}
	//落ちている
	else if (GetVel().y < 0.0f)
	{
		player_state_ = FALL;
	}

}

//サンダーはチャージ中は落ちない

void Player::Draw(Camera* camera)
{

	//GameObject::Draw(camera->GetCameraOffset());
	if (attack_attribute_) attack_attribute_->Draw(camera->GetCameraOffset());
}

void Player::DebugMenu()
{
	ImGui::Begin("Player");
	ImGui::Text("HP:%d", hp_);
	ImGui::Text("SkillPoint:%d", skillpt_);
	ImGui::Text("PlayerState:%d", player_state_);
	ImGui::End();
	if(move_attribute_ != nullptr) move_attribute_->DebugMenu();
	if(attack_attribute_ != nullptr) attack_attribute_->DebugMenu();
}


//================================================================================
// ↓パブリック関数↓
//================================================================================


void Player::CollisionMap(void)
{
	Map* map = GetMapMngr()->GetMap();
	Cell* cells[4] = {nullptr, nullptr, nullptr, nullptr};
	int idx = std::floor((GetPos().x / SIZE_));
	int idy = std::floor((GetPos().y / SIZE_));
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

		//地面の時の処理
		if (Collision(cells[i]) && i == 1)
		{
			MAP_READ cell_type = cells[i]->GetCellType();
			if (cell_type == MAP_READ_WALL || cell_type == MAP_READ_FLOOR)
			{
				player_state_ = TOUCH_GROUND;
				break;
			}
		}
	}
}
void Player::CollisionSpike(void)
{
	Map* map = GetMapMngr()->GetMap();
	Cell* cells[4] = { nullptr };
	int idx = std::floor(GetPos().x / SIZE_);
	int idy = std::floor(GetPos().y / SIZE_);
	cells[0] = map->GetCell(idx, idy + 1);	//頭
	cells[1] = map->GetCell(idx, idy - 1);	//足
	cells[2] = map->GetCell(idx - 1, idy);	//左
	cells[3] = map->GetCell(idx + 1, idy);	//右

	for (int i = 0; i < 4; i++)
	{
		if (cells[i] == nullptr)
			continue;

		//トゲの時の処理
		if (Collision(cells[i]))
		{
			MAP_READ cell_type = cells[i]->GetCellType();
			if (cell_type >= MAP_READ_SPIKE_LEFT && cell_type <= MAP_READ_SPIKE_DOWN)
			{
				knock_back_dir_ = i;
				clash_spike_ = SPIKE_SURPRISE_;
				switch (knock_back_dir_)
				{
				case 0:	//頭
					dir_.y = -1;
					break;
				case 1:	//足
					dir_.y = +1;
					break;
				case 2:	//左
					dir_.x = +1;
					break;
				case 3:	//右
					dir_.x = -1;
					break;
				default:
					break;
				}
				break;
			}
		}
	}

	Vector2 clash_vel(0.0f,0.0f);	//クラッシュしたときの速度
	if (clash_spike_ > 0)
	{
		float knock_back = 2.0f * clash_spike_;
		
		switch (knock_back_dir_)
		{
		case 0:	//頭
			clash_vel = Vector2(GetVel().x, dir_.y * knock_back);
			break;
		case 1:	//足
			clash_vel = Vector2(GetVel().x, dir_.y * knock_back);
			break;
		case 2:	//左
			clash_vel = Vector2(dir_.x * knock_back, GetVel().y);
			break;
		case 3:	//右
			clash_vel = Vector2(dir_.x * knock_back, GetVel().y);
			break;
		default:
			break;
		}

		SetVel(clash_vel);
		clash_spike_--;
	}


}

//上に移動
//落ちる
//地面にいる

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
