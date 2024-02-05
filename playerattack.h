#pragma once

#include <list>
#include "movableobj.h"
#include "time.h"
#include "lib/win_time.h"

class AttackHitEffect;

class PlayerAttack
{
private:

protected:
	std::list< AttackHitEffect*> hit_effects_;
	std::map<Collider*,WIN::Time> target_;
	float damage_ = 0;
	float tick_ = 1.0f;
	float knock_back_rate_ = 10.f;

public:
    PlayerAttack(float damage,float tick,float knock_back_rate) : damage_(damage),tick_(tick),knock_back_rate_(knock_back_rate) {}
    virtual ~PlayerAttack() = default;
    void SetDamage(float damage) { damage_ = damage; }
    float GetDamage() const { return damage_; }

	//エフェクト追加
	void AttachHitEffect(AttackHitEffect* hit_effect) {
		hit_effects_.push_back(hit_effect);
	}
	//エフェクトアップデート
	void HitEffectUpdate();

private:
	void RemoveEffect(void);
};

class AttackHitEffect : public MovableObj
{
private:
	float time_;
	bool draw_;

public:
	AttackHitEffect(Vector2 pos, Vector2 scale, textures texenum,LOOP_ANIM loop_anim)
		:MovableObj(Vector2::Zero, 0.0f, LoadTexture(Asset::GetAsset(texenum)), Vector2::Zero), time_(0.0f), draw_(false) {
		SetType(OBJ_VOID);
		GetCollider()->Discard();
		SetCollider(nullptr);
		GetAnimator()->SetTexenum(texenum);
		GetAnimator()->SetLoopAnim(loop_anim);
		GetAnimator()->SetDrawPriority(75);
		SetPos(pos);
		SetScale(scale);
	}

	bool GetDraw(void) { return draw_; }

	void Update(void) override {
		if (draw_) {
			time_ += Time::GetDeltaTime();
			if (time_ > 0.3f) {
				time_ = 0.0f;
				draw_ = false;
				Discard();
			}
		}
	}
};