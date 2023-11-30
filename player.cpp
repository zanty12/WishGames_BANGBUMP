//--------------------------------------------------------------------------------
// 
// �v���C���[[player.cpp]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/17
// �ŏI�X�V��	2023/11/20
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
	{//�������삵�Ȃ���Η�����
		if (GetVel().y <= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y - 0.05f));
	}

	if (attack_attribute_ != nullptr)
	{
		attack_attribute_->Action();
	}

	AddVel(GetVel());
	UpdateDir();
	CollisionMap();
	using namespace PHYSICS;

	Vertex4 square(Vector2(-1, 1), Vector2(1, 1), Vector2(1, -1), Vector2(-1, -1));
	Vertex1 point(Vector2(0, 0), 100);
	Collider2D::Touch(point, square);



}

//================================================================================
// ���p�u���b�N�֐���
//================================================================================


void Player::CollisionMap(void)
{
	Map* map = GetMapMngr()->GetMap();
	Cell* cells[4] = {nullptr, nullptr, nullptr, nullptr};
	int idx = std::floor(GetPos().x / size_);
	int idy = std::floor(GetPos().y / size_);
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
	}
}
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
