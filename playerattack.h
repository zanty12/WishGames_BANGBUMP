#pragma once

#include <list>
#include "movableobj.h"
#include "time.h"

#include "sound.h"

class AttackHitEffect;

class PlayerAttack
{
protected:
    float damage_;
    float damage_cd_ = 0.1f; //max time for each damage tick
    float cd_timer_ = 0.0f; //time for each damage tick
protected:
	std::list< AttackHitEffect*> hit_effects_;
	int attack_sound_ = 0;
	void LoadAttackSound(sounds sound) { attack_sound_ = LoadSound(Asset::GetAsset(sound).c_str()); }

public:
    PlayerAttack(float damage) : damage_(damage) {}
    virtual ~PlayerAttack();
    void SetDamage(float damage) { damage_ = damage; }
    float GetDamage() const { return damage_; }

	//エフェクト追加
	void AttachHitEffect(AttackHitEffect* hit_effect) {
		hit_effects_.push_back(hit_effect);
	}
	//エフェクトアップデート
	void HitEffectUpdate();
	//エフェクトが存在してるかどうか
	bool CheckHitEffect(void) { return hit_effects_.size(); }

	void StopSE(void) { StopSound(attack_sound_); }

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
		draw_ = true;
	}

	bool GetDraw(void) const { return draw_; }

	void Update(void) override {
		if (draw_) {
			time_ += Time::GetDeltaTime();
			if (time_ > 0.3f) {
				draw_ = false;
				Discard();
			}
		}
	}
};