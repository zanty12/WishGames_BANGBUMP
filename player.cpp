//--------------------------------------------------------------------------------
// 
// �v���C���[[player.cpp]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/17
// �ŏI�X�V��	2023/12/15
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
	//HP��0�ɂȂ����烊�U���g�Ɉڂ�
	if (hp_ <= 0)
	{
		change_scene_ = true;
	}

	//5�t���[�����ɉ������삵�Ă��Ȃ������痎���铮��Ɉڍs����
	if (Input::GetStickLeft(0).x == 0.0f && Input::GetStickLeft(0).y == 0.0f)
	{
		not_stick_working_++;
	}
	else
	{
		not_stick_working_ = 0;
	}

	//----------------------------------------���A�g���r���[�g���ł���܂ł̂��̂�
	/*Vector2 stick = Input::GetStickLeft(0);
	stick.y *= -1;
	Vector2 player_vel = stick * 5.0f;
	SetVel(player_vel);*/
	//----------------------------------------���A�g���r���[�g���ł���܂ł̂��̂�

	bool affected_gravity = false;	//�d�͂��󂯂����ǂ���

	Vector2 next_vel = GetVel();
	if (move_attribute_ != nullptr && clash_spike_ == 0)
	{
		next_vel = move_attribute_->Move();
		move_attribute_->Gravity();
	}
	else if (clash_spike_ == 0)
	{/*//�����������Ȃ���Η�����
		if (GetVel().y >= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y - 0.05f));*/
	}
	//�d��
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

	//��ɏオ���Ă���
	if (GetVel().y > 0.0f)
	{
		player_state_ = MOVE_UP;
	}
	//�����Ă���
	else if (GetVel().y < 0.0f)
	{
		player_state_ = FALL;
	}

}

void Player::DropSkillOrb(void)
{

	//��ATTRIBUTE_TYPE_FIRE���O�������Bbool isMovable = true�ɂ��遚

	while (true)
	{
		if (drop_point_ >= SKILLORB_SIZE_DESC::Big().value)	//�傫���X�L���I�[�u
		{
			map_mangr_->GetGame()->GetSkillOrbMngr()->Pop(GetPos(), ATTRIBUTE_TYPE_FIRE, SKILLORB_SIZE_TYPE_BIG);
			drop_point_ -= SKILLORB_SIZE_DESC::Big().value;
		}
		else if (drop_point_ >= SKILLORB_SIZE_DESC::Mid().value)	//�����炢�̃X�L���I�[�u
		{
			map_mangr_->GetGame()->GetSkillOrbMngr()->Pop(GetPos(), ATTRIBUTE_TYPE_FIRE, SKILLORB_SIZE_TYPE_MID);
			drop_point_ -= SKILLORB_SIZE_DESC::Mid().value;
		}
		else if (drop_point_ >= SKILLORB_SIZE_DESC::Small().value)	//�������X�L���I�[�u
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

//�T���_�[�̓`���[�W���͗����Ȃ�

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
// ���p�u���b�N�֐���
//================================================================================

//================================================================================
// �����������̃A�N�V����
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
// �X�L���|�C���g�ɓ����������̃A�N�V����
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
// �A�g���r���[�g�̃A�^�b�N�ɓ����������̃A�N�V����
//================================================================================
void Player::CollisionAttack(GameObject* obj)
{
	//���A�^�b�N�N���X���ł����恚
	GameObject* attack = obj;
	//��������̃A�^�b�N�N���X* attack = dynamic_cast<��������̃A�^�b�N�N���X*>(obj)
	//if (��������̃A�^�b�N�N���X == nullptr)
	//{
	//	return;
	//}

	hit_attack_attr = ATTRIBUTE_TYPE_FIRE; /*attack->GetAttribute()*/

	//�A�^�b�N�I�u�W�F�N�g��Rot���玩���������ׂ�����������o��
	dir_.x = cosf(attack->GetRot());
	dir_.y = sinf(attack->GetRot());

	switch (hit_attack_attr)
	{
	case ATTRIBUTE_TYPE_FIRE:
	{//1/3�b��2�}�X
		knockback_distance_ = SIZE_ * 2;
		knockback_time_ = 1.0f / 3;
		break;
	}
	case ATTRIBUTE_TYPE_THUNDER:
	{//3/4�b��3�}�X
		knockback_distance_ = SIZE_ * 3;
		knockback_time_ = 3.0f / 4;
		break;
	}
	case ATTRIBUTE_TYPE_WIND:
	{//1/4�b��1�}�X
		knockback_distance_ = SIZE_ * 1;
		knockback_time_ = 1.0f / 4;
		break;
	}
	case ATTRIBUTE_TYPE_DARK:
	{//1�b��1�}�X
		knockback_distance_ = SIZE_ * 1;
		knockback_time_ = 1.0f / 1;
		break;
	}
	default:
		break;
	}

	knockback_start_ = GetPos();
	knockback_end_ = GetPos() - (dir_ * knockback_distance_);

	SkillPointDown(0);	//���ۂɎ󂯂��_���[�W�����炷
	drop_point_ += 0;	//���ۂɎ󂯂��_���[�W��~�ς���
}

//================================================================================
// �g�Q�ɓ����������̃A�N�V����
//================================================================================
void Player::CollisionSpike(void)
{
	if (invincibility_time_ < INVINCIBILITY_MAX_TIME_)
	{
		//�߂荞�܂Ȃ��悤�Ɏ~�߂�
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

		//knockback_end_���o�O��̂ł��̂܂�
		knockback_start_ = GetPos();
		knockback_end_ = GetPos();
		return;
	}
	dir_ *= -1;	//���]������

	knockback_time_ = 1.0f / 4;
	knockback_distance_ = SIZE_;

	knockback_start_ = GetPos();
	knockback_end_ = GetPos() - (dir_ * knockback_distance_);
}

//================================================================================
// �G�l�~�[�ɓ����������̃A�N�V����
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
	dir_ *= -1;	//���]������

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
// ���G���Ԃ̃A�N�V����
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
	// �_��
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
	// �m�b�N�o�b�N
	//----------------------------------------
	//���`���
	Vector2 lerp = knockback_start_ + (knockback_end_ - knockback_start_) * (invincibility_time_ / knockback_time_);
	SetPos(lerp);
}

//================================================================================
// ���x���A�b�v
//================================================================================
void Player::LvUp(void)
{
	for (int i = 0; i < LV_NUM; i++)
	{
		if (skillpt_ > LvUpPoint[i])
		{
			if (lv_ < i + 1)	//Lv�����͂��Ȃ�
			{
				lv_ = i + 1;
				HpMaxUp();
			}
		}
	}

}

//================================================================================
// �ő�HP�̕ω�
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
���`��� vector2_1 + (vector2_2 - vector2_1) * float
*/

/*
	MAP_READ_ORB_SMALL_FIRE, ///< ���X�L���ʃZ����\���܂��B
	MAP_READ_ORB_MID_FIRE, ///< ���X�L���ʃZ����\���܂��B
	MAP_READ_ORB_BIG_FIRE, ///< ��X�L���ʃZ����\���܂��B
	MAP_READ_ORB_SMALL_DARK, ///< ���X�L���ʃZ����\���܂��B
	MAP_READ_ORB_MID_DARK, ///< ���X�L���ʃZ����\���܂��B
	MAP_READ_ORB_BIG_DARK, ///< ��X�L���ʃZ����\���܂��B
	MAP_READ_ORB_SMALL_WIND, ///< ���X�L���ʃZ����\���܂��B
	MAP_READ_ORB_MID_WIND, ///< ���X�L���ʃZ����\���܂��B
	MAP_READ_ORB_BIG_WIND, ///< ��X�L���ʃZ����\���܂��B
	MAP_READ_ORB_SMALL_THUNDER, ///< ���X�L���ʃZ����\���܂��B
	MAP_READ_ORB_MID_THUNDER, ///< ���X�L���ʃZ����\���܂��B
	MAP_READ_ORB_BIG_THUNDER, ///< ��X�L���ʃZ����\���܂��B
*/

//��Ɉړ�
//������
//�n�ʂɂ���

/*
	OBJ_SOLID, //�ђʕs�\
	OBJ_PENETRABLE, //���ђ�
	OBJ_VOID, //�ђʉ\
	OBJ_SPIKE, //�g�Q
	OBJ_PLAYER, //�v���C���[
	OBJ_ENEMY, //�G
	OBJ_ATTACK, //�U��
*/

/*
MAP_READ_NONE, ///< ��̃Z����\���܂��B
MAP_READ_WALL, ///< �ǃZ����\���܂��B
MAP_READ_FLOOR, ///< ���Z����\���܂��B
MAP_READ_PENFLOOR, ///< �ђʉ\�ȏ��Z����\���܂��B
MAP_READ_CLOUD, ///< �_�Z����\���܂��B
MAP_READ_THUNDERCLOUD, ///< ���_�Z����\���܂��B
MAP_READ_BLOCK, ///< �u���b�N�Z����\���܂��B
MAP_READ_ORB_SMALL, ///< ���X�L���ʃZ����\���܂��B
MAP_READ_ORB_MID, ///< ���X�L���ʃZ����\���܂��B
MAP_READ_ORB_BIG, ///< ��X�L���ʃZ����\���܂��B
MAP_READ_SPIKE_LEFT, ///< �������̃g�Q�Z����\���܂��B
MAP_READ_SPIKE_RIGHT, ///< �E�����̃g�Q�Z����\���܂��B
MAP_READ_SPIKE_UP, ///< ������̃g�Q�Z����\���܂��B
MAP_READ_SPIKE_DOWN, ///< �������̃g�Q�Z����\���܂��B
*/
