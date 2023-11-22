#pragma once
#include "gameobject.h"
#include "sprite.h"
#include "mapenum.h"

class Cell : public GameObject
{
	//特にやることない
public:
	Cell() = delete;
	Cell(int x, int y, int tex) : GameObject(Vector2(x* GameObject::size_ + GameObject::size_ / 2, y* GameObject::size_ + GameObject::size_ / 2), 0.0f, tex) {} //ゲーム座標からマス座標に変換
	virtual ~Cell() = default;
	void Update() override {}
	void Draw() override
	{
		DrawSprite(this->GetTexNo(),this->GetPos(),this->GetRot(),Vector2(GameObject::size_,GameObject::size_),Color(1.0f,1.0f,1.0f,1.0f));
	}
	virtual MAP_READ GetCellType() const = 0;
};
