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

#define LV_NUM (10)

static const int LvUpPoint[LV_NUM] =
{
	0,
	20,
	30,
	40,
	50,
	60,
	90,
	120,
	140,
	160,
};


bool Player::LvUp(int get_skill_pt)
{


	for (int i = 0; i < LV_NUM; i++)
	{
		if (skillpt_ > LvUpPoint[i])
		{
			if (lv_ < i)	//Lv下げはしない
			{
				lv_ = i;
			}
		}
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

	//----------------------------------------★アトリビュートができるまでのしのぎ
	Vector2 stick = Input::GetStickLeft(0);
	stick.y * -1;
	Vector2 player_vel = stick * 5.0f;
	SetVel(player_vel);
	//----------------------------------------★アトリビュートができるまでのしのぎ

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

	CollisionAction();

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

void Player::CollisionAction(void)
{
	std::list<Collider*> collisions = GetCollider()->GetCollision();
	for (auto collision : collisions)
	{
		OBJECT_TYPE type = collision->GetParent()->GetType();
		switch (type)
		{
		case OBJ_SOLID:
			if (GetVel().x != 0.0f)
				SetVel(Vector2(0.0f, GetVel().y));
			if (GetVel().y != 0.0f)
				SetVel(Vector2(GetVel().x, 0.0f));
			break;
		case OBJ_PENETRABLE:
			if (GetVel().x != 0.0f)
				SetVel(Vector2(0.0f, GetVel().y));
			if (GetVel().y < 0.0f)
				SetVel(Vector2(GetVel().x, 0.0f));
			break;
		case OBJ_SPIKE:
			CollisionSpike();
			break;
		case OBJ_PLAYER:
			break;
		case OBJ_ENEMY:	//とりあえず止まるようにする
			if (GetVel().x != 0.0f)
				SetVel(Vector2(0.0f, GetVel().y));
			if (GetVel().y != 0.0f)
				SetVel(Vector2(GetVel().x, 0.0f));
			break;
		case OBJ_ATTACK:
			break;
		default:
			break;
		}
	}
}

void Player::CollisionSpike(void)
{
	if (GetVel().x > 0.0f)
		dir_.x = -1;
	if (GetVel().x < 0.0f)
		dir_.x = 1;
	if (GetVel().y > 0.0f)
		dir_.y = -1;
	if (GetVel().y < 0.0f)
		dir_.y = 1;

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
	OBJ_SOLID, //貫通不能
	OBJ_PENETRABLE, //下貫通
	OBJ_VOID, //貫通可能
	OBJ_SPIKE, //トゲ
	OBJ_PLAYER, //プレイヤー
	OBJ_ENEMY, //敵
	OBJ_ATTACK, //攻撃
*/

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
