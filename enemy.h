#pragma once
#include "movableobj.h"
#include "mapmngr.h"
#include "skillorb.h"

#include "time.h"

class EnemyMngr;



enum ENEMY_TYPE
{
    TYPE__KOOPA,
    TYPE__HAMMERBRO,
    TYPE__PHANTOM,
};

class EnemyDeadEffect;

class Enemy : public MovableObj
{
private:
    static const float BLLINKING_MAX_TIME_;	//点滅時間
    EnemyMngr* enemy_mngr_;
    ENEMY_TYPE enemy_type_;
    float flash_time_;			//点滅間隔
    float blinking_time_;	//点滅の経過時間
    int atk_;
    int hp_;

    bool flashing_; //点滅
protected:
    EnemyDeadEffect* dead_effect_ = nullptr;

public:
    Enemy() = delete;
    Enemy(int x, int y,int texNo,EnemyMngr* enemy_mngr) : MovableObj(Vector2((x + 0.5f) * SIZE_, (y + 0.5f) * SIZE_),0.0f,texNo,Vector2(0.0f,0.0f)),flashing_(false)
    {
        enemy_mngr_ = enemy_mngr;
    }
    virtual ~Enemy() = default;
    virtual void Update() = 0;
    EnemyMngr* GetEnemyMngr() const { return enemy_mngr_; }


    void DropSkillOrb(Vector2 pos, SKILLORB_SIZE_TYPE sizeType);

    ENEMY_TYPE GetEnemyType() const { return enemy_type_; }

    int GetAtk(void) const { return atk_; }
    void SetAtk(int atk) { atk_ = atk; }
    int GetHp(void) const { return hp_; }
    void SetHp(int hp) { hp_ = hp; }
    //HPの減少（ダメージが現在のHPを超える場合、HPは0になる）
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

    void blinking(void);
    void Flashing(void) { flashing_ = true; }
    bool GetFlashing(void) { return flashing_; }
};

class EnemyDeadEffect : public MovableObj
{
private:
    float time_;

public:
    EnemyDeadEffect(Vector2 pos)
        :MovableObj(pos, 0.0f, LoadTexture(Asset::GetAsset(effect_enemydead)),Vector2::Zero), time_(0.0f) {
        SetType(OBJ_VOID);
        GetAnimator()->SetTexenum(effect_enemydead);
        GetAnimator()->SetLoopAnim(EFFECT_ENEMYDEAD_ANIM);
    }

    bool EffectEnd(void) {
        time_ += Time::GetDeltaTime();
        if (time_ > 1.3f) {
            return true;
        }
        return false;
    }

    void Update(void) override {}
};
