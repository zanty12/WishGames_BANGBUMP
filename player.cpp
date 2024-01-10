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
	0  ,
	10 ,
	25 ,
	45 ,
	70 ,
	100,
	145,
	205,
	275,
	355,
};

const int Player::INITIAL_HP_ = 500;
const float Player::INVINCIBILITY_MAX_TIME_ = 1 + (1.0f / 4);


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
	/*Vector2 stick = Input::GetStickLeft(0);
	stick.y *= -1;
	Vector2 player_vel = stick * 5.0f;
	SetVel(player_vel);*/
	//----------------------------------------★アトリビュートができるまでのしのぎ

	bool affected_gravity = false;	//重力を受けたかどうか

	Vector2 next_vel = GetVel();
	if (move_attribute_ != nullptr && clash_spike_ == 0)
	{
		next_vel = move_attribute_->Move();
		move_attribute_->Gravity();
	}
	else if (clash_spike_ == 0)
	{/*//何も属性がなければ落ちる
		if (GetVel().y >= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y - 0.05f));*/
	}
	//重力
	if(GetGravityState() == GRAVITY_FULL)
	{
		SetVel(Vector2(next_vel.x, next_vel.y - GRAVITY_SCALE_ * Time::GetDeltaTime() * Time::GetDeltaTime()));
	}
	else if (GetGravityState() == GRAVITY_HALF)
	{
		SetVel(Vector2(next_vel.x, next_vel.y - GRAVITY_SCALE_ / 2 * Time::GetDeltaTime() * Time::GetDeltaTime()));
	}
	else if (GetGravityState() == GRAVITY_NONE)
	{
		SetVel(Vector2(next_vel.x, next_vel.y));
	}

	if (attack_attribute_ != nullptr)
	{
		attack_attribute_->Action();
	}

	UpdateDir();

	AddVel(GetVel());

	CollisionAction();

	Invincibility();

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

void Player::DropSkillOrb(void)
{

	//★ATTRIBUTE_TYPE_FIREを外すだけ。bool isMovable = trueにする★

	while (true)
	{
		if (drop_point_ >= SKILLORB_SIZE_DESC::Big().value)	//大きいスキルオーブ
		{
			map_mangr_->GetGame()->GetSkillOrbMngr()->Pop(GetPos(), ATTRIBUTE_TYPE_FIRE, SKILLORB_SIZE_TYPE_BIG);
			drop_point_ -= SKILLORB_SIZE_DESC::Big().value;
		}
		else if (drop_point_ >= SKILLORB_SIZE_DESC::Mid().value)	//中くらいのスキルオーブ
		{
			map_mangr_->GetGame()->GetSkillOrbMngr()->Pop(GetPos(), ATTRIBUTE_TYPE_FIRE, SKILLORB_SIZE_TYPE_MID);
			drop_point_ -= SKILLORB_SIZE_DESC::Mid().value;
		}
		else if (drop_point_ >= SKILLORB_SIZE_DESC::Small().value)	//小さいスキルオーブ
		{
			map_mangr_->GetGame()->GetSkillOrbMngr()->Pop(GetPos(), ATTRIBUTE_TYPE_FIRE, SKILLORB_SIZE_TYPE_SMALL);
			drop_point_ -= SKILLORB_SIZE_DESC::Small().value;
		}

		if (drop_point_ <= 0)
		{
			break;
		}
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

//================================================================================
// 当たった時のアクション
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
			/*if (GetVel().x != 0.0f)
				SetVel(Vector2(0.0f, GetVel().y));
			if (GetVel().y != 0.0f)
				SetVel(Vector2(GetVel().x, 0.0f));*/
			if (invincibility_time_ < 1.0f / 4)
				invincibility_time_ = 1.0f / 4;
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
		case OBJ_ENEMY:
		{
			GameObject* gameObj = collision->GetParent();
			CollisionEnemy(gameObj);
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

//================================================================================
// スキルポイントに当たった時のアクション
//================================================================================
void Player::CollisionSkillPoint(GameObject* obj)
{
	if (attack_attribute_ == nullptr || move_attribute_ == nullptr)
	{
		return;
	}


	SkillOrbMovable* skill_point = dynamic_cast<SkillOrbMovable*>(obj);
	if(skill_point == nullptr)
		SkillOrbStatic* skill_point = dynamic_cast<SkillOrbStatic*>(obj);

	if (skill_point == nullptr)
	{
		return;
	}

	switch (skill_point->GetSize())
	{
	case SKILLORB_SIZE_TYPE_SMALL:
	{
		int point = SKILLORB_SIZE_DESC::Small().value;
		skillpt_ += (point * 4);
		break;
	}
	case SKILLORB_SIZE_TYPE_MID:
	{
		int point = SKILLORB_SIZE_DESC::Mid().value;
		skillpt_ += (point * 4);
		break;
	}
	case SKILLORB_SIZE_TYPE_BIG:
	{
		int point = SKILLORB_SIZE_DESC::Big().value;
		skillpt_ += (point * 4);
		break;
	}
	default:
		break;
	}

	skill_point->Discard();
}

//================================================================================
// アトリビュートのアタックに当たった時のアクション
//================================================================================
void Player::CollisionAttack(GameObject* obj)
{
	//★アタッククラスができ次第★
	GameObject* attack = obj;
	//何かしらのアタッククラス* attack = dynamic_cast<何かしらのアタッククラス*>(obj)
	//if (何かしらのアタッククラス == nullptr)
	//{
	//	return;
	//}

	hit_attack_attr = ATTRIBUTE_TYPE_FIRE; /*attack->GetAttribute()*/

	//アタックオブジェクトのRotから自分が動くべき方向を割り出す
	dir_.x = cosf(attack->GetRot());
	dir_.y = sinf(attack->GetRot());

	switch (hit_attack_attr)
	{
	case ATTRIBUTE_TYPE_FIRE:
	{//1/3秒で2マス
		knockback_distance_ = SIZE_ * 2;
		knockback_time_ = 1.0f / 3;
		break;
	}
	case ATTRIBUTE_TYPE_THUNDER:
	{//3/4秒で3マス
		knockback_distance_ = SIZE_ * 3;
		knockback_time_ = 3.0f / 4;
		break;
	}
	case ATTRIBUTE_TYPE_WIND:
	{//1/4秒で1マス
		knockback_distance_ = SIZE_ * 1;
		knockback_time_ = 1.0f / 4;
		break;
	}
	case ATTRIBUTE_TYPE_DARK:
	{//1秒で1マス
		knockback_distance_ = SIZE_ * 1;
		knockback_time_ = 1.0f / 1;
		break;
	}
	default:
		break;
	}

	knockback_start_ = GetPos();
	knockback_end_ = GetPos() - (dir_ * knockback_distance_);

	SkillPointDown(0);	//実際に受けたダメージ分減らす
	drop_point_ += 0;	//実際に受けたダメージを蓄積する
}

//================================================================================
// トゲに当たった時のアクション
//================================================================================
void Player::CollisionSpike(void)
{
	if (invincibility_time_ < INVINCIBILITY_MAX_TIME_)
	{
		//めり込まないように止める
		if (GetVel().x != 0.0f)
			SetVel(Vector2(0.0f, GetVel().y));
		if (GetVel().y != 0.0f)
			SetVel(Vector2(GetVel().x, 0.0f));
		return;
	}

	SkillPointDown(5);

	invincibility_time_ = 0.0f;

	dir_ = GetVel().Normalize();
	if (abs(GetVel().x) < 0.1f && abs(GetVel().y < 0.1f))
	{
		if (GetVel().x != 0.0f)
			SetVel(Vector2(0.0f, GetVel().y));
		if (GetVel().y != 0.0f)
			SetVel(Vector2(GetVel().x, 0.0f));

		//knockback_end_がバグるのでそのまま
		knockback_start_ = GetPos();
		knockback_end_ = GetPos();
		return;
	}
	dir_ *= -1;	//反転させる

	knockback_time_ = 1.0f / 4;
	knockback_distance_ = SIZE_;

	knockback_start_ = GetPos();
	knockback_end_ = GetPos() - (dir_ * knockback_distance_);
}

//================================================================================
// エネミーに当たった時のアクション
//================================================================================
void Player::CollisionEnemy(GameObject* obj)
{
	Enemy* enemy = dynamic_cast<Enemy*>(obj);

	if (enemy == nullptr)
	{
		return;
	}
	if (invincibility_time_ < INVINCIBILITY_MAX_TIME_)
	{
		return;
	}

	invincibility_time_ = 0.0f;

	dir_ = GetVel().Normalize();
	if (abs(GetVel().x) < 0.1f && abs(GetVel().y < 0.1f))
	{
		dir_ = -enemy->GetVel().Normalize();
	}
	dir_ *= -1;	//反転させる

	SkillPointDown(enemy->GetAtk());


	switch (enemy->GetEnemyType())
	{
	case TYPE__KOOPA:
		knockback_time_ = 1.0f / 4;
		knockback_distance_ = SIZE_;
		break;
	case TYPE__HAMMERBRO:
		knockback_time_ = 1.0f / 4;
		knockback_distance_ = SIZE_;
		break;
	case TYPE__PHANTOM:
		knockback_time_ = 1.0f / 4;
		knockback_distance_ = SIZE_ * 3;
		break;
	default:
		break;
	}

	knockback_start_ = GetPos();
	knockback_end_ = GetPos() - (dir_ * knockback_distance_);

}

//================================================================================
// 無敵時間のアクション
//================================================================================
void Player::Invincibility(void)
{
	invincibility_time_ += Time::GetDeltaTime();

	if (invincibility_time_ > INVINCIBILITY_MAX_TIME_)
	{
		SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		return;
	}

	//----------------------------------------
	// 点滅
	//----------------------------------------
	flash_time_ += Time::GetDeltaTime();
	if (flash_time_ > 0.1f)
	{
		flash_time_ = 0.0f;
		if (GetColor().a == 1.0f)
			SetColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
		else
			SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (invincibility_time_ > knockback_time_)
	{
		return;
	}

	//----------------------------------------
	// ノックバック
	//----------------------------------------
	//線形補間
	Vector2 lerp = knockback_start_ + (knockback_end_ - knockback_start_) * (invincibility_time_ / knockback_time_);
	SetPos(lerp);
}

//================================================================================
// レベルアップ
//================================================================================
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

//================================================================================
// 最大HPの変化
//================================================================================
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
		hp_ = (int)(INITIAL_HP_ * (1.1f + (0.1f * lv_)));
	}
	else
	{
		hp_ = (int)(INITIAL_HP_ * (1 + (0.1f * lv_)));
	}

}


/*
線形補間 vector2_1 + (vector2_2 - vector2_1) * float
*/

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
