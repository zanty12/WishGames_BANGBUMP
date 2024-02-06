#include "enemy2.h"
#include "bullet.h"
#include "Cell.h"
#include "MapMngr.h"
#include "playerattack.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 10.0f)                            //範囲


bool CheckEnemy2Length(Vector2 a, Vector2 b, float len);


void Enemy2::Update()
{
    //HPが0になったら消す
    if (GetHp() <= 0)
    {
        SetColor(Color(0, 0, 0, 0));    //透明にする
        SetType(OBJ_VOID);  //当たり判定を消す
        GetAnimator()->SetIsAnim(false);    //アニメーションをしない
        if (dead_effect_ == nullptr)
        {
            dead_effect_ = new EnemyDeadEffect(GetPos());   //エフェクト生成
            dead_effect_->SetScale(GetScale());
        }

        DropSkillOrb(GetPos(), SKILLORB_SIZE_TYPE_MID);
    }

    //消滅エフェクト
    if (dead_effect_)
    {
        //エフェクトが終了したらDiscard
        if (dead_effect_->EffectEnd())
        {
            delete dead_effect_;
            dead_effect_ = nullptr;
            Discard();
        }

        //エネミーには何もさせない
        return;
    }

    float dt = Time::GetDeltaTime(); //初期化時のエラーを回避する
    atk_time_ += Time::GetDeltaTime();

    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
        switch (type)
        {
        case OBJ_ATTACK:
            {

            }
            break;
            default:
                break;
        }
    }

    GetAnimator()->SetIsAnim(true);

    std::list<Player*> players = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayers();

    float Spos_now = 0.0f;
    float Spos_old = 10000.0f;

    Player* close_player = nullptr;
    Bullet* bullet = nullptr;

    for (auto player : players)
    {
        if (CheckEnemy2Length(startPosition, player->GetPos(), RANGE))
        {
            Spos_now = Vector2::Distance(GetPos(), player->GetPos());
            if (Spos_now < Spos_old)
            {
                close_player = player;
                Spos_old = Spos_now;
            }
        }
    }

    if (close_player != nullptr)
    {
        if (atk_time_ > 2.0f)
        {
            atk_time_ = 0;
            Vector2 bullet_pos;
            bullet = new Bullet(GetPos());
            Vector2 distance = close_player->GetPos() - startPosition;
            bullet_pos = distance.Normalize();
            bullet->SetVel(distance.Normalize() * (96.0f * 2) * dt);
            bullet->SetPos(startPosition + (bullet_pos * SIZE_ * 1.5f));
            GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(bullet);
        }
    }
}


bool CheckEnemy2Length(Vector2 a, Vector2 b, float len)
{
    if (Vector2::Distance(a, b) < len)
    {
        return true;
    }

    return false;
}
