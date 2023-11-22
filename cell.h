#pragma once
#include "gameobject.h"
#include "sprite.h"

class Cell : public GameObject
{
	//���ɂ�邱�ƂȂ�
public:
	Cell() = delete;
	Cell(int x, int y, int tex) : GameObject(Vector2(x* GameObject::size_ + GameObject::size_ / 2, y* GameObject::size_ + GameObject::size_ / 2), 0.0f, tex) {} //�Q�[�����W����}�X���W�ɕϊ�
	virtual ~Cell() = default;
	void Update() override {}
	void Draw() override
	{
		DrawSprite(this->GetTexNo(),this->GetPos(),this->GetRot(),Vector2(10.0f,10.0f),Color(1.0f,1.0f,1.0f,1.0f));
	}
};
