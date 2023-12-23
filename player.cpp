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
#include "skillorb.h"
#include "enemy.h"
#include "xinput.h"

#define LV_NUM (10)

static const int LvUpPoint[LV_NUM] =
{
	0,
	20,
	50,
	90,
	140,
	200,
	290,
	410,
	550,
	710,
};



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

	LvUp();

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

SkillOrb* Player::DropSkillOrb(void)
{
	SKILLORB_ATTRIBUTE_DESC skillorb_attr;
	switch (hit_attack_attr)
	{
	case ATTRIBUTE_TYPE_FIRE:
		skillorb_attr = SKILLORB_ATTRIBUTE_DESC::Fire();
		break;
	case ATTRIBUTE_TYPE_DARK:
		skillorb_attr = SKILLORB_ATTRIBUTE_DESC::Dark();
		break;
	case ATTRIBUTE_TYPE_WIND:
		skillorb_attr = SKILLORB_ATTRIBUTE_DESC::Wind();
		break;
	case ATTRIBUTE_TYPE_THUNDER:
		skillorb_attr = SKILLORB_ATTRIBUTE_DESC::Thunder();
		break;
	}

	if (drop_point_ >= 10)
	{
		drop_point_ -= 10;
		return new SkillOrb(GetPos(), skillorb_attr, SKILLORB_SIZE_DESC::Big());
	}
	if (drop_point_ >= 3)
	{
		drop_point_ -= 3;
		return new SkillOrb(GetPos(), skillorb_attr, SKILLORB_SIZE_DESC::Mid());
	}
	if (drop_point_ >= 1)
	{
		drop_point_ -= 1;
		return new SkillOrb(GetPos(), skillorb_attr, SKILLORB_SIZE_DESC::Small());
	}

	return nullptr;
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
	ImGui::Text("LV:%d", lv_);
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
		{
			if (GetVel().x != 0.0f)
				SetVel(Vector2(0.0f, GetVel().y));
			if (GetVel().y != 0.0f)
				SetVel(Vector2(GetVel().x, 0.0f));
			GameObject* gameObj = collision->GetParent();
			Enemy* enemy = dynamic_cast<Enemy*>(gameObj);

			break;
		}
		case OBJ_ATTACK:
		{
			GameObject* attack = collision->GetParent();
			if (attack != nullptr)
			{
				CollisionAttack(attack);
			}

			break;
		}
		case OBJ_ITEM:
		{
			GameObject* skillPoint = collision->GetParent();
			if (skillPoint != nullptr)
			{
				CollisionSkillPoint(skillPoint);
			}
			break;
		}
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

void Player::CollisionSkillPoint(GameObject* obj)
{
	if (attack_attribute_ == nullptr || move_attribute_ == nullptr)
	{
		return;
	}

	SkillOrb* skill_point = dynamic_cast<SkillOrb*>(obj);

	if (skill_point == nullptr)
	{
		return;
	}

	ATTRIBUTE_TYPE pt_attr = skill_point->GetAttribute();	//スキルポイント属性
	SKILLORB_SIZE_TYPE pt_size = skill_point->GetSize();	//スキルポイントサイズ

	if (pt_size == SKILLORB_SIZE_TYPE_BIG)
	{
		skillpt_ += 40;		//10ポイント * 4
		skill_point->Discard();
		return;
	}

	if (pt_attr == move_attribute_->GetAttribute() || pt_attr == attack_attribute_->GetAttribute())
	{
		//小さいスキルポイントの時
		if (pt_size == SKILLORB_SIZE_TYPE_SMALL)
			skillpt_ += 8;	//3ポイント * 2 + 1ポイント * 2
		//中くらいのスキルポイントの時
		if (pt_size == SKILLORB_SIZE_TYPE_MID)
			skillpt_ += 16;	//5ポイント * 2 + 3ポイント * 2
	}
	else
	{
		//小さいスキルポイントの時
		if (pt_size == SKILLORB_SIZE_TYPE_SMALL)
			skillpt_ += 4;	//1ポイント * 4
		//中くらいのスキルポイントの時
		if (pt_size == SKILLORB_SIZE_TYPE_MID)
			skillpt_ += 12;	//3ポイント * 4
	}

	skill_point->Discard();
}

void Player::CollisionAttack(GameObject* obj)
{
	Attribute* attack = dynamic_cast<Attribute*>(obj);

	if (attack == nullptr || attack == attack_attribute_)//そんなことはないかもしれないけど念のため
	{
		return;
	}

	hit_attack_attr = attack->GetAttribute();

	SkillPointDown(0);	//実際に受けたダメージ分減らす
	drop_point_ += 0;	//実際に受けたダメージを蓄積する
}

void Player::LvUp(void)
{
	for (int i = 0; i < LV_NUM; i++)
	{
		if (skillpt_ > LvUpPoint[i])
		{
			if (lv_ < i + 1)	//Lv下げはしない
			{
				lv_ = i + 1;
				HpMaxUp();
			}
		}
	}

}

void Player::HpMaxUp(void)
{
	if (lv_ == 1)
		return;

	if (lv_ == 5)
	{
		hp_ = 775;
		return;
	}

	if (lv_ < 5)
	{
		hp_ = INITIAL_HP_ * (1.1f + (0.1f * lv_));
	}
	else
	{
		hp_ = INITIAL_HP_ * (1 + (0.1f * lv_));
	}

}


/*
	MAP_READ_ORB_SMALL_FIRE, ///< 小スキル玉セルを表します。
	MAP_READ_ORB_MID_FIRE, ///< 中スキル玉セルを表します。
	MAP_READ_ORB_BIG_FIRE, ///< 大スキル玉セルを表します。
	MAP_READ_ORB_SMALL_DARK, ///< 小スキル玉セルを表します。
	MAP_READ_ORB_MID_DARK, ///< 中スキル玉セルを表します。
	MAP_READ_ORB_BIG_DARK, ///< 大スキル玉セルを表します。
	MAP_READ_ORB_SMALL_WIND, ///< 小スキル玉セルを表します。
	MAP_READ_ORB_MID_WIND, ///< 中スキル玉セルを表します。
	MAP_READ_ORB_BIG_WIND, ///< 大スキル玉セルを表します。
	MAP_READ_ORB_SMALL_THUNDER, ///< 小スキル玉セルを表します。
	MAP_READ_ORB_MID_THUNDER, ///< 中スキル玉セルを表します。
	MAP_READ_ORB_BIG_THUNDER, ///< 大スキル玉セルを表します。
*/

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
