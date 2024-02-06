#include "playerattack.h"

void PlayerAttack::HitEffectUpdate()
{
	RemoveEffect();
	for (auto hit_effect : hit_effects_) {
		hit_effect->Update();
	}
}

void PlayerAttack::RemoveEffect(void)
{
	hit_effects_.remove_if(
		[](AttackHitEffect* hit_effect) {
			if (hit_effect->GetDiscard()) {
				delete hit_effect;
				hit_effect = nullptr;
				return true;
			}
			return false;
		}
	);
}
