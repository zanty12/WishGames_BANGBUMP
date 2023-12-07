#pragma once
#include "gameobject.h"
#include "sprite.h"
#include "mapenum.h"

class Cell : public GameObject
{
	//���ɂ�邱�ƂȂ�
public:
	Cell() = delete;
	Cell(int x, int y, int tex) : GameObject(Vector2(x* GameObject::SIZE_ + GameObject::SIZE_ / 2, y* GameObject::SIZE_ + GameObject::SIZE_ / 2), 0.0f, tex) {} //�Q�[�����W����}�X���W�ɕϊ�
	virtual ~Cell() = default;
	void Update() override {}
	virtual MAP_READ GetCellType() const = 0;
};
