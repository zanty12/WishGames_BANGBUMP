//--------------------------------------------------------------------------------
// 
// プレイヤー[player.h]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/16
// 最終更新日	2023/11/30
// 
//--------------------------------------------------------------------------------

#pragma once

#include "animator.h"
#include "movableobj.h"
#include "attribute.h"
#include "collidercir.h"
#include "mapmngr.h"
#include "playerstate.h"

class RevolveEffect;

enum GRAVITY_STATE
{
    GRAVITY_FULL,
    GRAVITY_HALF,
    GRAVITY_NONE,
};

class MapMngr;
class Camera;
class SkillOrb;
class PlayerDeadEffect;
class PlayerHitEffect;

class Player : public MovableObj
{
private:
    static const int INITIAL_HP_; //HPの初期値

    static const float INVINCIBILITY_MAX_TIME_; //無敵時間

    Vector2 dir_; //向き

    class Attribute* move_attribute_ = nullptr; //動く用のアトリビュート
    class Attribute* attack_attribute_ = nullptr; //攻撃用のアトリビュート

    int hp_;
    int skillpt_;
    int lv_;

    int drop_point_; //落とすポイント
    ATTRIBUTE_TYPE hit_attack_attr = ATTRIBUTE_TYPE_FIRE; //受けた攻撃属性

    bool change_scene_; //シーン遷移フラグ

    MapMngr* map_mangr_;

    int clash_spike_; //トゲに衝突したら15フレームの間ノックバック
    int knock_back_dir_; //トゲに衝突した方向

    float invincibility_time_; //無敵の経過時間
    float flash_time_; //点滅間隔
    float knockback_time_; //ノックバックする時間
    float knockback_distance_; //ノックバックする距離
    Vector2 knockback_start_; //ノックバックの初めのポジション
    Vector2 knockback_end_; //ノックバックの終わりのポジション

    int not_stick_working_;
    float revolve_cd_ = 2.0f;

    PLAYER_STATE player_state_;
    GRAVITY_STATE gravity_state_ = GRAVITY_FULL;
    RevolveEffect* revolve_effect_ = nullptr;

protected:
    PlayerDeadEffect* dead_effect_ = nullptr;
    PlayerHitEffect* hit_effect_ = nullptr;

public:
    Player(Vector2 pos, float rot, Vector2 vel, MapMngr* map_mangr);
    ~Player();

    void SetDir(Vector2 dir) { dir_ = dir; } //向きのセット
    Vector2 GetDir(void) const { return dir_; } //向きのゲット
    int GetHp(void) const { return hp_; } //HPのゲット
    void SetHp(int hp) { hp_ = hp; } //HPのセット
    void SetAttribute(Attribute* move_attribute)
    {
        delete move_attribute_;
        move_attribute_ = move_attribute;
    } //ムーブアトリビュートポインタのセット（何も操作していないときはnullptrをセット）
    void SetAttackAttribute(Attribute* attack_attribute)
    {
        delete attack_attribute_;
        attack_attribute_ = attack_attribute;
    } //アタックアトリビュートポインタのセット（何も操作していないときはnullptrをセット）
    Attribute* GetAttribute(void) const { return move_attribute_; } //ムーブアトリビュートポインタをゲット（属性が何もなければnullptrを返す）
    Attribute* GetAttackAttribute(void) const { return attack_attribute_; } //アタックアトリビュートポインタをゲット（属性が何もなければnullptrを返す）
    void SetMapMngr(MapMngr* map_mangr) { map_mangr_ = map_mangr; } //MapMngrをセット
    MapMngr* GetMapMngr(void) const { return map_mangr_; } //MapMngrのポインタをゲット
    bool GetChangeSceneFlag(void) const { return change_scene_; } //シーンチェンジのフラグ true=別のシーンへ
    PLAYER_STATE GetPlayerState(void) const { return player_state_; } //プレイヤーのステータスをゲット
    int GetSkillPoint(void) const { return skillpt_; } //現在所有しているスキルポイントのゲット
    int GetLv(void) const { return lv_; } //現在のレベルのゲット

    //スキルポイントの減少（ダメージが所持スキルポイントを超える場合、スキルポイントは0になる）
    void SkillPointDown(int damage) { damage <= skillpt_ ? skillpt_ -= damage : skillpt_ = 0; }
    //HPの減少（ダメージが現在のHPを超える場合、HPは0になる）
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

    void DropSkillOrb(void);

    void Update(void) override;
    void Draw(Camera* camera);

    void DebugMenu();

    void SetGravityState(GRAVITY_STATE gravity_state) { gravity_state_ = gravity_state; }
    GRAVITY_STATE GetGravityState(void) const { return gravity_state_; }
    static constexpr float GRAVITY_SCALE_ = 3 * SIZE_; //重力

private:
    //向きのアップデート。速度をもとに更新（全く動いていない場合は止まった瞬間の向きのままにする）
    void UpdateDir(void) { if (GetVel() != Vector2(0.0f, 0.0f)) dir_ = GetVel().Normalize(); }

    //何かに当たったときのアクション
    void CollisionAction(void);
    //当たり判定（スキルポイント）
    void CollisionSkillPoint(GameObject* obj);
    //当たり判定（アタックアトリビュート）
    void CollisionAttack(GameObject* obj);
    //当たり判定（トゲ）
    void CollisionSpike(void);
    //当たり判定（エネミー）
    void CollisionEnemy(GameObject* obj);
    //当たり判定（エネミー２バレット）
    void CollisionBullet(GameObject* obj);

    //無敵の時
    void Invincibility(void);

    //レベルアップ（ゲットしたスキルポイントを引数にする）
    void LvUp(void);

    void HpMaxUp(void);
    void Revolve(void);
};


class PlayerDeadEffect : public MovableObj
{
private:
    float time_;

public:
    PlayerDeadEffect(Vector2 pos)
        : MovableObj(pos, 0.0f, LoadTexture(Asset::GetAsset(effect_enemydead)), Vector2::Zero), time_(0.0f)
    {
        SetType(OBJ_VOID);
        GetAnimator()->SetTexenum(effect_dead);
        GetAnimator()->SetLoopAnim(EFFECT_DEAD_ANIM);
    }

    bool EffectEnd(void)
    {
        time_ += Time::GetDeltaTime();
        if (time_ > 2.5f)
        {
            return true;
        }
        return false;
    }

    void Update(void) override
    {
    }
};


class PlayerHitEffect : public MovableObj
{
private:
    float time_;
    bool draw_;

public:
    PlayerHitEffect(void)
        : MovableObj(Vector2::Zero, 0.0f, LoadTexture(Asset::GetAsset(effect_hit)), Vector2::Zero), time_(0.0f),
          draw_(false)
    {
        SetType(OBJ_VOID);
        GetCollider()->Discard();
        SetCollider(nullptr);
        GetAnimator()->SetTexenum(effect_hit);
        GetAnimator()->SetLoopAnim(EFFECT_HIT_ANIM);
        GetAnimator()->SetDrawPriority(75);
        SetScale(Vector2(SIZE_ * 1.5f, SIZE_ * 1.5f));
    }

    void Hit(Vector2 my_pos)
    {
        draw_ = true;
        SetPos(my_pos);

        SetColor(Color(1, 1, 1, 1));
        GetAnimator()->SetIsAnim(true);
        GetAnimator()->SetColor(GetColor());
    }

    bool GetDraw(void) { return draw_; }

    void Update(void) override
    {
        if (draw_)
        {
            time_ += Time::GetDeltaTime();
            if (time_ > 0.3f)
            {
                time_ = 0.0f;
                draw_ = false;
                DispUninit();
            }
        }
    }
};

class RevolveEffect : public MovableObj
{
private:
    float time_;
    Player* player_;

public:
    RevolveEffect(Player* player) : MovableObj(player->GetPos(), 0.0f, LoadTexture("data/texture/effect/revolve.png"),
                                               Vector2::Zero), time_(0.0f), player_(player)
    {
        SetType(OBJ_VOID);
        SetScale(Vector2(SIZE_ * 2.0f, SIZE_ * 2.0f));
        SetColor(Color(1, 1, 1, 1));
        GetCollider()->Discard();
        SetCollider(nullptr);
        GetAnimator()->SetTexenum(effect_EX);
        GetAnimator()->SetLoopAnim(EFFECT_EX_ANIM);
        GetAnimator()->SetDrawPriority(75);
    }
    ~RevolveEffect() override = default;
    void Update()
    {
        if(time_ < 1.0f)
        {
            time_ += Time::GetDeltaTime();
            SetPos(player_->GetPos());
        }
    }
};
