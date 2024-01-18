#pragma once
#include <string>
#include <list>
#include <map>
#include "lib/vector.h"
#include "multi_object.h"

class MultiGameObject;
class MultiMap {
private:
	int width = 0;
	int height = 0;
	int *map = nullptr;
	
	MultiBehavior *gameObjects;
	std::map<int, int> texNumbers;

public:
	Vector2 position;
	float cellSize = 50;
	std::list<Vector2> startPosition;



public:
	void Initialize(void);
	void Release(void);
	Vector2Int ToIndex(Vector2 pos) { return Vector2Int(pos.x / cellSize, pos.y / cellSize); }
	Vector2 ToPosition(Vector2Int idx) { return Vector2(idx.x * cellSize, idx.y * cellSize); }
	void Load(std::string path);
	void Draw(Vector2 offset);
	int Collision(Vector2 &position, float radius, Vector2 *normal);
	int &GetMap(int x, int y) { return map[x + y * width]; }

private:
};