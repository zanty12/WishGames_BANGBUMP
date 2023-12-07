#pragma once
#include "gameobject.h"
#include "sprite.h"
#include "mapenum.h"

class Cell : public GameObject
{
	//特にやることない
public:
	Cell() = delete;
	Cell(int x, int y, int tex) : GameObject(Vector2(x* GameObject::SIZE_ + GameObject::SIZE_ / 2, y* GameObject::SIZE_ + GameObject::SIZE_ / 2), 0.0f, tex) {} //ゲーム座標からマス座標に変換
	virtual ~Cell() = default;
	void Update() override {}
	virtual MAP_READ GetCellType() const = 0;
};
