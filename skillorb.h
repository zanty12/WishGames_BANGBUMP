#pragma once
#include "cell.h"
#include "mapenum.h"

typedef enum
{
	ORB_SMALL = 1,
	ORB_MID,
	ORB_BIG,
}ORB_TYPE;

class SkillOrb :
	public Cell
{
private:
	ORB_TYPE type_;
	float size_ = GameObject::size_;
public:
	SkillOrb(int x, int y, MAP_READ type) : Cell(x, y, 0) //TODO : change 0 to SkillOrb type
	{
		switch (type)
		{
		case MAP_READ_ORB_SMALL:
			type_ = ORB_SMALL;
			break;
		case MAP_READ_ORB_MID:
			type_ = ORB_MID;
			this->SetPos(this->GetPos() + Vector2(GameObject::size_ / 2,-GameObject::size_ / 2)); //2*2ÇÃê^ÇÒíÜÇ…Ç∏ÇÍÇÈ
			size_ = GameObject::size_ * type_;
			break;
		case MAP_READ_ORB_BIG:
			type_ = ORB_BIG;
			this->SetPos(this->GetPos() + Vector2(GameObject::size_, -GameObject::size_)); //3*3ÇÃê^ÇÒíÜÇ…Ç∏ÇÍÇÈ
			size_ = GameObject::size_ * type_;
			break;
		default:
			break;
		}
	} 
	virtual ~SkillOrb() override = default;
};
