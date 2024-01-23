#pragma once
#include "movableobj.h"
#include "mapmngr.h"
#include "skillorb.h"
#include "playerattack.h"

class EnemyMngr;



enum ENEMY_TYPE
{
    TYPE__KOOPA,
    TYPE__HAMMERBRO,
    TYPE__PHANTOM,
};


class Enemy : public MovableObj
{
private:
    static const float BLLINKING_MAX_TIME_;	//�_�Ŏ���
    EnemyMngr* enemy_mngr_;
    ENEMY_TYPE enemy_type_;
    float flash_time_;			//�_�ŊԊu
    float blinking_time_;	//�_�ł̌o�ߎ���
    int atk_;
    int hp_;
public:
    Enemy() = delete;
    Enemy(int x, int y,int texNo,EnemyMngr* enemy_mngr) : MovableObj(Vector2((x + 0.5f) * SIZE_, (y + 0.5f) * SIZE_),0.0f,texNo,Vector2(0.0f,0.0f))
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
    //HP�̌����i�_���[�W�����݂�HP�𒴂���ꍇ�AHP��0�ɂȂ�j
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

    void blinking(GameObject* obj);

    void blinking(PlayerAttack* obj);

};
