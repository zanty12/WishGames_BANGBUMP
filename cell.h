#pragma once
#include "gameobject.h"

typedef enum
{
	CELL_TYPE_NONE,
	CELL_TYPE_WALL,
	CELL_TYPE_FLOOR,
	CELL_TYPE_PENFLOOR,
	CELL_TYPE_CLOUD,
	CELL_TYPE_THUNDERCLOUD,
	CELL_TYPE_BLOCK,
	CELL_TYPE_ORB_SMALL,
	CELL_TYPE_ORB_MID,
	CELL_TYPE_ORB_BIG,
	CELL_TYPE_MAX
}CELL_TYPE;

class Cell : public GameObject
{
	//特にやることない
public:
	Cell() = delete;
	Cell(int x, int y, int tex) : GameObject(Vector2(x* GameObject::size_ + GameObject::size_ / 2, y* GameObject::size_ + GameObject::size_ / 2), 0.0f, tex) {} //ゲーム座標からマス座標に変換
	virtual ~Cell() = default;
	void Update() override {}
	void Draw() override {}
};
