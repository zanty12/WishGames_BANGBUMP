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
			if (lv_ < i)	//Lv�����͂��Ȃ�
			{
				lv_ = i;
			}
		}
	}

	return false;
}

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
	Vector2 stick = Input::GetStickLeft(0);
	stick.y * -1;
	Vector2 player_vel = stick * 5.0f;
	SetVel(player_vel);
	//----------------------------------------���A�g���r���[�g���ł���܂ł̂��̂�

	bool affected_gravity = false;	//�d�͂��󂯂����ǂ���

	if (move_attribute_ != nullptr && clash_spike_ == 0)
	{
		(void)move_attribute_->Move();
		move_attribute_->Gravity();

	}
	else if (clash_spike_ == 0)
	{//�����������Ȃ���Η�����
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

//�T���_�[�̓`���[�W���͗����Ȃ�

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
// ���p�u���b�N�֐���
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
		case OBJ_ENEMY:	//�Ƃ肠�����~�܂�悤�ɂ���
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

	Vector2 clash_vel(0.0f,0.0f);	//�N���b�V�������Ƃ��̑��x
	if (clash_spike_ > 0)
	{
		float knock_back = 2.0f * clash_spike_;
		
		switch (knock_back_dir_)
		{
		case 0:	//��
			clash_vel = Vector2(GetVel().x, dir_.y * knock_back);
			break;
		case 1:	//��
			clash_vel = Vector2(GetVel().x, dir_.y * knock_back);
			break;
		case 2:	//��
			clash_vel = Vector2(dir_.x * knock_back, GetVel().y);
			break;
		case 3:	//�E
			clash_vel = Vector2(dir_.x * knock_back, GetVel().y);
			break;
		default:
			break;
		}

		SetVel(clash_vel);
		clash_spike_--;
	}
}

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
