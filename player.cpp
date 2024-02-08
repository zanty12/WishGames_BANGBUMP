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
#include "bullet.h"

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



Player::Player(Vector2 pos, float rot, Vector2 vel, MapMngr* map_mangr)
	:MovableObj(pos, rot, 0, vel), hp_(INITIAL_HP_), skillpt_(0), lv_(1),
	dir_(Vector2(0.0f, 0.0f)), map_mangr_(map_mangr), clash_spike_(0), knock_back_dir_(0),
	change_scene_(false), drop_point_(0), invincibility_time_(INVINCIBILITY_MAX_TIME_),
	knockback_distance_(0.0f), knockback_time_(0.0f)
{
	SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));
	int tex = LoadTexture("data/texture/player.png");
	SetTexNo(tex);
	GetAnimator()->SetTexNo(tex);
	SetType(OBJ_PLAYER);

	hit_effect_ = new PlayerHitEffect();
	//�\���D��x�ݒ�
	GetAnimator()->SetDrawPriority(50);
}

Player::~Player()
{
	delete move_attribute_;
	delete attack_attribute_;
	delete hit_effect_;
}


void Player::Update(void)
{

	//HP��0�ɂȂ����烊�U���g�Ɉڂ�
	if (hp_ <= 0)
	{
		SetColor(Color(0, 0, 0, 0));	//�����ɂ���
		SetType(OBJ_VOID);  //�����蔻�������
		GetAnimator()->SetIsAnim(false);	//�A�j���[�V���������Ȃ�
		delete move_attribute_;
		move_attribute_ = nullptr;
		delete attack_attribute_;
		attack_attribute_ = nullptr;

		delete hit_effect_;
		hit_effect_ = nullptr;

		if (dead_effect_ == nullptr)
		{
			dead_effect_ = new PlayerDeadEffect(GetPos());	//�G�t�F�N�g����
			dead_effect_->SetScale(GetScale());
		}
	}

	//���ŃG�t�F�N�g
	if (dead_effect_)
	{
		//�G�t�F�N�g���I��������Discard
		if (dead_effect_->EffectEnd())
		{
			delete dead_effect_;
			dead_effect_ = nullptr;
			Discard();

			change_scene_ = true;
		}

		//�v���C���[�ɂ͉��������Ȃ�
		return;
	}

	//�q�b�g�G�t�F�N�g�A�b�v�f�[�g
	if (hit_effect_)
	{
		hit_effect_->Update();
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


	bool affected_gravity = false;	//�d�͂��󂯂����ǂ���

	if (attack_attribute_ != nullptr)
	{
		attack_attribute_->Action();
	}

	Vector2 next_vel = GetVel();
	if (move_attribute_ != nullptr && clash_spike_ == 0)
	{
		next_vel = move_attribute_->Move();
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

	invincibility_time_ += Time::GetDeltaTime();

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

	//revolve
	if(revolve_cd_ > 0.0f)
	{
		revolve_cd_ -= Time::GetDeltaTime();
	}
	if(Input::GetStickLeft(0).x > 0.8f && Input::GetStickRight(0).x < -0.8f)
	{
		revolve_cd_ -= Time::GetDeltaTime();
		if(revolve_cd_ <= 0)
		{
			Revolve();
			revolve_cd_ = 2.0f;
			//revolve_effect_ = new RevolveEffect(this);
		}
	}

	if (revolve_effect_)
	{
		revolve_effect_->Update();
	}

	//limit player pos
	if (GetPos().y < this->GetScale().y)
	{
		SetPos(Vector2(GetPos().x, this->GetScale().y));
	}
	if(GetPos().x < this->GetScale().x)
	{
		SetPos(Vector2( this->GetScale().x, GetPos().y));
	}
	if(GetPos().x > map_mangr_->GetMap()->GetWidth()*SIZE_ - this->GetScale().x)
	{
		SetPos(Vector2(map_mangr_->GetMap()->GetWidth() * SIZE_ - this->GetScale().x, GetPos().y));
	}
	if(GetPos().y > map_mangr_->GetMap()->GetHeight()*SIZE_  - this->GetScale().y)
	{
		SetPos(Vector2(GetPos().x, map_mangr_->GetMap()->GetHeight() * SIZE_  - this->GetScale().y));
	}
}

void Player::DropSkillOrb(void)
{

	//��ATTRIBUTE_TYPE_FIRE���O�������Bbool isMovable = true�ɂ��遚

	while (true)
	{
		if (drop_point_ >= SKILLORB_SIZE_DESC::Big().value)	//�傫���X�L���I�[�u
		{
			map_mangr_->GetGame()->GetSkillOrbMngr()->Pop(GetPos(), SKILLORB_SIZE_TYPE_BIG,true);
			drop_point_ -= SKILLORB_SIZE_DESC::Big().value;
		}
		else if (drop_point_ >= SKILLORB_SIZE_DESC::Mid().value)	//�����炢�̃X�L���I�[�u
		{
			map_mangr_->GetGame()->GetSkillOrbMngr()->Pop(GetPos(), SKILLORB_SIZE_TYPE_MID,true);
			drop_point_ -= SKILLORB_SIZE_DESC::Mid().value;
		}
		else if (drop_point_ >= SKILLORB_SIZE_DESC::Small().value)	//�������X�L���I�[�u
		{
			map_mangr_->GetGame()->GetSkillOrbMngr()->Pop(GetPos(), SKILLORB_SIZE_TYPE_SMALL,true);
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
}

void Player::DebugMenu()
{
	ImGui::Begin("Player");
	ImGui::Text("LV:%d", lv_);
	ImGui::Text("HP:%d", hp_);
	ImGui::Text("VelX%f", GetVel().x);
	ImGui::Text("VelY%f", GetVel().y);
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
			player_state_ = TOUCH_GROUND;

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
		{
			CollisionSpike();
			GameObject* gameObj = collision->GetParent();
			break;
		}
		case OBJ_PLAYER:
			break;
		case OBJ_ENEMY:
		{
			GameObject* gameObj = collision->GetParent();
			CollisionEnemy(gameObj);
			break;
		}
		case OBJ_BULLET:
		{
			GameObject* gameObj = collision->GetParent();
			CollisionBullet(gameObj);
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
	//�G�t�F�N�g�̕\��


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

	//�G�t�F�N�g�̕\��
	hit_effect_->Hit(GetPos());

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
		dir_ = GetPos() - enemy->GetPos();
		dir_ = dir_.Normalize();
		dir_ *= -1;
		break;
	case TYPE__PHANTOM:
		knockback_time_ = 1.0f / 4;
		knockback_distance_ = SIZE_ * 3;
		break;
	default:
		break;
	}

	if (dir_ != Vector2::Zero)
	{
		knockback_start_ = GetPos();
		knockback_end_ = GetPos() - (dir_ * knockback_distance_);
	}
	else
	{
		knockback_start_ = GetPos();
		knockback_end_ = GetPos();
	}

	//�G�t�F�N�g�̕\��
	hit_effect_->Hit(GetPos());

}

//================================================================================
// �G�l�~�[�Q�̃o���b�g�ɓ����������̃A�N�V����
//================================================================================
void Player::CollisionBullet(GameObject* obj)
{
	Bullet* bullet = dynamic_cast<Bullet*>(obj);

	if (bullet == nullptr)
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
		dir_ = bullet->GetVel().Normalize();
	}
	dir_ *= -1;

	SkillPointDown(bullet->GetAtk());

	knockback_time_ = 1.0f / 4;
	knockback_distance_ = SIZE_;

	if (dir_ != Vector2::Zero)
	{
		knockback_start_ = GetPos();
		knockback_end_ = GetPos() - (dir_ * knockback_distance_);
	}
	else
	{
		knockback_start_ = GetPos();
		knockback_end_ = GetPos();
	}

	//�G�t�F�N�g�̕\��
	hit_effect_->Hit(GetPos());
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

void Player::Revolve()
{
	Attribute* temp = move_attribute_;
	move_attribute_ = attack_attribute_;
	attack_attribute_ = temp;
	//load texture
	switch (this->GetAttackAttribute()->GetAttribute())
    {
    case ATTRIBUTE_TYPE_FIRE:
        switch (this->GetAttribute()->GetAttribute())
        {
        case ATTRIBUTE_TYPE_DARK:
            this->GetAnimator()->SetTexenum(player1_12);
            break;
        case ATTRIBUTE_TYPE_WIND:
            this->GetAnimator()->SetTexenum(player1_14);
            break;
        case ATTRIBUTE_TYPE_THUNDER:
            this->GetAnimator()->SetTexenum(player1_13);
            break;
        default:
            break;
        }
        break;
    case ATTRIBUTE_TYPE_DARK:
        switch (this->GetAttribute()->GetAttribute())
        {
        case ATTRIBUTE_TYPE_FIRE:
            this->GetAnimator()->SetTexenum(player1_21);
            break;
        case ATTRIBUTE_TYPE_WIND:
            this->GetAnimator()->SetTexenum(player1_24);
            break;
        case ATTRIBUTE_TYPE_THUNDER:
            this->GetAnimator()->SetTexenum(player1_23);
            break;
        default:
            break;
        }
        break;
    case ATTRIBUTE_TYPE_WIND:
        switch (this->GetAttribute()->GetAttribute())
        {
        case ATTRIBUTE_TYPE_FIRE:
            this->GetAnimator()->SetTexenum(player1_41);
            break;
        case ATTRIBUTE_TYPE_DARK:
            this->GetAnimator()->SetTexenum(player1_42);
            break;
        case ATTRIBUTE_TYPE_THUNDER:
            this->GetAnimator()->SetTexenum(player1_43);
            break;
        default:
            break;
        }
        break;
    case ATTRIBUTE_TYPE_THUNDER:
        switch (this->GetAttribute()->GetAttribute())
        {
        case ATTRIBUTE_TYPE_FIRE:
            this->GetAnimator()->SetTexenum(player1_31);
            break;
        case ATTRIBUTE_TYPE_DARK:
            this->GetAnimator()->SetTexenum(player1_32);
            break;
        case ATTRIBUTE_TYPE_WIND:
            this->GetAnimator()->SetTexenum(player1_34);
            break;
        default:
            break;
        }
        break;
    default:
        break;
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
