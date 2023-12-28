#include "enemy.h"
#include "camera.h"

ENEMY_TYPE Enemy::GetEnemyType(MAP_READ type)
{
	int iBase = MAP_READ_KOOPA;
	int iType = (type - iBase) % 2;
	return (ENEMY_TYPE)iType;
}
