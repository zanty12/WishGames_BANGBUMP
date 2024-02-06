#pragma once

#include <list>
#include "movableobj.h"
#include "time.h"

class AttackHitEffect;

class PlayerAttack
{
private:
    float damage_;
    float max_tick_ = 0.1f; //max time for each damage tick
    float tick_ = 0.0f; //time for each damage tick
protected:
	std::list< AttackHitEffect*> hit_effects_;

public:
    PlayerAttack(float damage) : damage_(damage) {}
    virtual ~PlayerAttack() = default;
    void SetDamage(float damage) { damage_ = damage; }
    float GetDamage() const { return damage_; }
    void SetTick(float tick) { tick_ = tick; }
    float GetTick() const { return tick_; }
    void SetMaxTick(float max_tick) { max_tick_ = max_tick; }
    float GetMaxTick() const { return max_tick_; }
    void UpdateTick() { tick_ += Time::GetDeltaTime(); }

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