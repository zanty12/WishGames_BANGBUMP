//--------------------------------------------------------------------------------
// 
// �v���C���[[player.cpp]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/17
// �ŏI�X�V��	2023/11/30
// 
//--------------------------------------------------------------------------------

#include "player.h"
#include "lib/collider2d.h"
#include "spike.h"

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

	//HP��0�ɂȂ����烊�U���g�Ɉڂ�
	if (hp_ <= 0)
	{
		change_scene_ = true;
	}

	if (move_attribute_ != nullptr && clash_spike_ == 0)
	{
		SetVel(move_attribute_->Move());
	}
	else if(clash_spike_ == 0)
	{//�������삵�Ȃ���Η�����
		if (GetVel().y <= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y - 0.05f));
		player_state_ = FALL;
	}

	if (attack_attribute_ != nullptr)
	{
		attack_attribute_->Action();
	}


	UpdateDir();

	AddVel(GetVel());

	CollisionMap();

	CollisionSpike();

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

void Player::Draw(Camera* camera)
{
	GameObject::Draw(camera->GetCameraOffset());
}


//================================================================================
// ���p�u���b�N�֐���
//================================================================================


void Player::CollisionMap(void)
{
	Map* map = GetMapMngr()->GetMap();
	Cell* cells[4] = {nullptr, nullptr, nullptr, nullptr};
	int idx = std::floor((GetPos().x / size_));
	int idy = std::floor((GetPos().y / size_));
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

		//�n�ʂ̎��̏���
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
	int idx = std::floor(GetPos().x / size_);
	int idy = std::floor(GetPos().y / size_);
	cells[0] = map->GetCell(idx, idy + 1);	//��
	cells[1] = map->GetCell(idx, idy - 1);	//��
	cells[2] = map->GetCell(idx - 1, idy);	//��
	cells[3] = map->GetCell(idx + 1, idy);	//�E

	for (int i = 0; i < 4; i++)
	{
		if (cells[i] == nullptr)
			continue;

		//�g�Q�̎��̏���
		if (Collision(cells[i]))
		{
			MAP_READ cell_type = cells[i]->GetCellType();
			if (cell_type >= MAP_READ_SPIKE_LEFT && cell_type <= MAP_READ_SPIKE_DOWN)
			{
				knock_back_dir_ = i;
				clash_spike_ = SPIKE_SURPRISE_;
				switch (knock_back_dir_)
				{
				case 0:	//��
					dir_.y = -1;
					break;
				case 1:	//��
					dir_.y = +1;
					break;
				case 2:	//��
					dir_.x = +1;
					break;
				case 3:	//�E
					dir_.x = -1;
					break;
				default:
					break;
				}
				break;
			}
		}
	}

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
