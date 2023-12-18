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
#include "xinput.h"

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

	if (move_attribute_ != nullptr && clash_spike_ == 0)
	{
		Vector2 move = move_attribute_->Move();
		if (abs(move.x) > 0.1f || move.y <= GRAVITY_SCALE_-0.05f || move.y > 0.1f)
		{
			SetVel(move);
		}
		else
		{
			if (not_stick_working_ > 5)
			{
				if (GetVel().y >= 0.0f)
					SetVel(Vector2(0.0f, GetVel().y - 0.5f));
				if (GetVel().y >= GRAVITY_SCALE_)
					SetVel(Vector2(0.0f, GetVel().y - 0.05f));
				affected_gravity = true;
			}
			else
			{
				SetVel(Vector2::Zero);
			}
		}
	}
	else if (clash_spike_ == 0)
	{//�����������Ȃ���Η�����
		if (GetVel().y >= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y - 0.05f));
	}

	//�����鏈�����܂����Ă��Ȃ��ăX�e�B�b�N��������Ă��Ȃ��Ƃ��͏d�͂�^����
	if (!affected_gravity && not_stick_working_ > 5)
	{
		if (GetVel().y >= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y - 0.05f));
	}

	if (attack_attribute_ != nullptr)
	{
		attack_attribute_->Action();
	}

	UpdateDir();

	AddVel(GetVel());

	//CollisionMap();

	//CollisionSpike();

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


/*void Player::CollisionMap(void)
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
	int idx = std::floor(GetPos().x / SIZE_);
	int idy = std::floor(GetPos().y / SIZE_);
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


}*/

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
