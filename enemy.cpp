#include "enemy.h"
#include "camera.h"

void Enemy::DropSkillOrb(Vector2 pos, SKILLORB_SIZE_TYPE sizeType)
{
	enemy_mngr_->GetMapMngr()->GetGame()->GetSkillOrbMngr()->Pop(pos, ATTRIBUTE_TYPE_FIRE, sizeType);
}
