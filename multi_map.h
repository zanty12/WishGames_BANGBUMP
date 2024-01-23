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
	int *collMap = nullptr;
	

	MultiBehavior *skillOrbs;
	MultiBehavior *enemies;
	MultiBehavior *attacks;
	std::list<Vector2> areaCaptures;
	std::map<int, int> texNumbers;

public:
	Vector2 position;
	float cellSize = 53;
	std::list<Vector2> startPosition;
	int frontBGTexNo = -1, backBGTexNo = -1;



public:
	MultiMap() = default;
	MultiMap(std::string path) { Initialize(); Load(path); };

	void Initialize(void);
	void Release(void);
	void Load(std::string path);
	Vector2Int ToIndex(Vector2 pos) { return Vector2Int(pos.x / cellSize, pos.y / cellSize); }
	Vector2 ToPosition(Vector2Int idx) { return Vector2(idx.x * cellSize, idx.y * cellSize); }
	void Draw(Vector2 offset);
	void AttakUpdate(void);
	int Collision(Vector2 &position, float radius);
	int &GetMap(int x, int y) { return map[x + y * width]; }
	int &GetColliderMap(int x, int y) { return collMap[x + y * width]; }
	MultiBehavior *GetSkillOrbs(void) { return skillOrbs; }
	MultiBehavior *GetEnemies(void) { return enemies; }
	MultiBehavior *GetAttacks(void) { return attacks; }
	std::list<Vector2> GetAreaCaptures(void) { return areaCaptures; }

private:
};