//--------------------------------------------------------------------------------
// 
// ワープ[fark.cpp]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
// 
//--------------------------------------------------------------------------------
#include"dark.h"

#include "asset.h"
#include"xinput.h"
#include"lib/collider2d.h"


Dark::Dark(Player* player)
    : Attribute(player, ATTRIBUTE_TYPE_DARK),move_effect_(new DarkEffect(this))
{
    LoadMoveSound(SE_dark_move);
    SetVolume(move_sound_, 1.0f);
}

Vector2 Dark::Move()
{
    Vector2 velocity = player_->GetVel();
    Vector2 stick = Input::GetStickLeft(0);

    if (stick.x > 0.0f)
    {
        player_->GetAnimator()->DirRight();
    }
    else if (stick.x < 0.0f)
    {
        player_->GetAnimator()->DirLeft();
    }

    Vector2 stickR = Input::GetStickRight(0);
    if (abs(stick.x) < 0.01f && abs(stick.y) < 0.01f &&
        abs(stickR.x) < 0.01f && abs(stickR.y) < 0.01f)
    {
        if (player_->GetAnimator()->GetLoopAnim() != PLAYER_ATTACK_ANIM &&
            player_->GetAnimator()->GetLoopAnimNext() != PLAYER_ATTACK_ANIM)
            player_->GetAnimator()->SetLoopAnim(PLAYER_IDLE_ANIM);
    }

    if (stick != Vector2::Zero)
    {
        if (move_indicator_ == nullptr)
        {
            move_indicator_ = new DarkIndicator();
        }
        Vector2 dir = stick.Normalize();

        Vector2 target_pos = player_->GetPos() + dir * warpDistance_ / 2;
        move_indicator_->SetPos(player_->GetPos());
        move_indicator_->SetTargetPos(target_pos);
        move_indicator_->Update();
    }

    else
    {
        delete move_indicator_;
        move_indicator_ = nullptr;
    }
    if (Input::GetKey(0, Input::LThumb))
    {
        move_effect_->Charge();

        if (velocity.y <= -maxSpeedFalling)
        {
            velocity.y = maxSpeedFalling;
            return velocity;
        }
    }
    if (Input::GetKeyUp(0, Input::LThumb))
    {
        //SE再生
        PlaySound(move_sound_, 0);

        move_effect_->Move();

        Vector2 stick = Input::GetStickLeft(0);
        stick = stick.Normalize();
        stick.y *= -1;
        Vector2 warpDirection = stick * warpDistance_;
        if (move_indicator_)
        {
            warpPosition = move_indicator_->GetPos();
        }

        player_->SetPos(warpPosition);
        delete move_indicator_;
        move_indicator_ = nullptr;
    }
    else warpPosition = Vector2::Zero;

    move_effect_->Update();

    return player_->GetVel();
};

void Dark::Action()
{
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);
    stick.y *= -1;
    //float distance = stick.Distance();
    if(attack_ != nullptr)
    {
        attack_->Update();
    }
    if (stick != Vector2::Zero)
    {
        if (attack_indicator_ == nullptr)
        {
            attack_indicator_ = new DarkAttackIndicator();
            attack_indicator_->GetAnimator()->SetColor(Color(0.0f, 0.0f, 1.0f, 1.0f));
        }
        Vector2 dir = stick.Normalize();
        float angle = atan2(dir.y, dir.x);
        Vector2 pos = Vector2(cos(angle), -sin(angle)) * (player_->GetScale().x / 2 + attack_indicator_->GetScale().x /
            2);
        pos = player_->GetPos() + pos;
        attack_indicator_->SetPos(pos);
        attack_indicator_->SetRot(angle);
    }
    else
    {
        delete attack_indicator_;
        attack_indicator_ = nullptr;
    }
    if (attack_)
        attack_->Update();

    // 押し込む
    if (Input::GetKeyDown(0, Input::RThumb) && responseMinStickDistance < stick.Distance())
    {
        attackDirection_ = stick.Normalize();

        if (stick.x > 0.0f)
        {
            player_->GetAnimator()->DirRight();
        }
        else if (stick.x < 0.0f)
        {
            player_->GetAnimator()->DirLeft();
        }
    }
    // 攻撃
    else if (Input::GetKey(0, Input::RThumb))
    {
        player_->GetAnimator()->SetLoopAnim(PLAYER_ATTACK_ANIM);

        if (attack_ == nullptr)
            attack_ = new DarkAttack(this);
        float angle = atan2(attackDirection_.y, attackDirection_.x);
        Vector2 pos = Vector2(cos(angle), -sin(angle)) * (player_->GetScale().x / 2 + attack_->GetScale().x / 2 + 0.5 * GameObject::SIZE_);
        pos = player_->GetPos() + pos;
        attack_->SetPos(pos);
        attack_->SetRot(angle + (3.14f / 2));
        delete attack_indicator_;
        attack_indicator_ = nullptr;
    }
    else
    {
        if (attack_ != nullptr)
        {
            player_->GetAnimator()->SetLoopAnim(PLAYER_IDLE_ANIM);

            if (attack_ != nullptr)
            {
                if (!attack_->CheckHitEffect())
                {
                    delete attack_;
                    attack_ = nullptr;
                }
            }
        }
    }
}

void Dark::DebugMenu()
{
    ImGui::Begin("Dark");
    ImGui::SliderFloat2("maxSpeedFalling", &maxSpeedFalling, 0.0f, 1.0f);
    ImGui::SliderFloat2("warpDistance", &warpDistance_, 400.0f, 1000.0f);
    ImGui::End();
}

void Dark::Gatchanko(bool is_attack)
{
    //アタック時は移動エフェクトを消す
    if (is_attack)
    {
        delete move_indicator_;
        move_indicator_ = nullptr;
        move_effect_->DispUninit();
    }
    //移動時なら攻撃エフェクトおよびオブジェクトを消す。移動エフェクトの再表示
    else
    {
        delete attack_;
        attack_ = nullptr;
        delete attack_indicator_;
        attack_indicator_ = nullptr;
        move_effect_->DispInit();
    }
}

DarkAttack::DarkAttack(Dark* parent) : parent_(parent),
                                       MovableObj(
                                           Vector2(
                                               parent->GetPlayer()->GetPos().x + parent->GetPlayer()->GetScale().x / 2 +
                                               5 * GameObject::SIZE_, parent->GetPlayer()->GetPos().y), 0.0f,
                                           LoadTexture(Asset::GetAsset(dark_attack)), Vector2::Zero),PlayerAttack(10000)
{
    SetScale(Vector2(SIZE_*3, SIZE_ * 6));
    SetType(OBJ_ATTACK);
    SetDamage(parent->GetState()->atk);
    damage_cd_ = parent->GetState()->atkCoolTime;

    //アニメーション設定
    GetAnimator()->SetTexenum(dark_attack);
    GetAnimator()->SetLoopAnim(DARK_ATTACK_ANIM);
    GetAnimator()->SetDrawPriority(75);

    //サウンド
    LoadAttackSound(SE_dark_attack);
    //SE再生
    PlaySound(attack_sound_, -1);
}

void DarkAttack::Update()
{
    if(cd_timer_ > 0.0f)
    {
        cd_timer_ -= Time::GetDeltaTime();
    }
    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
        switch (type)
        {
        case OBJ_ENEMY:
            {
                Enemy* enemy = dynamic_cast<Enemy*>(collision->GetParent());
                if (enemy != nullptr)
                {
                    if (cd_timer_ <= 0.0f)
                    {
                        cd_timer_ = damage_cd_;
                        enemy->SetHp(enemy->GetHp() - GetDamage());

                        if (!enemy->GetDiscard())
                        {
                            Vector2 pos = enemy->GetPos();
                            Vector2 scale = enemy->GetScale();
                            AttachHitEffect(new AttackHitEffect(pos, scale, effect_hit_dark, EFFECT_HIT_DARK_ANIM));
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    HitEffectUpdate();  //エフェクトのアップデート
}

DarkIndicator::DarkIndicator() : MovableObj(Vector2::Zero, 0.0f, LoadTexture(Asset::GetAsset(effect_water_indicator)), Vector2::Zero)
{
    SetScale(Vector2(2 * GameObject::SIZE_, 2 * GameObject::SIZE_));
    GetAnimator()->SetColor(Color(0.0f, 0.0f, 0.5f, 0.5f));
    SetType(OBJ_VOID);

    //アニメーション設定
    GetAnimator()->SetTexenum(effect_water_indicator);
    GetAnimator()->SetLoopAnim(EFFECT_WATER_INDICATOR);
    GetAnimator()->SetDrawPriority(25);
}

void DarkIndicator::Update()
{
    bool hit = false;
    //try collision at target_pos_
    ColliderRect* target_collider = dynamic_cast<ColliderRect*>(GetCollider());
    ColliderRect* temp= new ColliderRect(*target_collider);
    temp->SetPos(target_pos_);
    GameBase::GetCollMngr()->UpdateCollision(temp);
    if(temp->GetCollision().size() == 0 || temp->GetCollision().front()->GetParent()->GetType() == OBJ_VOID)
    {
        //std::cout<<"target_pos_ is valid"<<std::endl;
        SetPos(target_pos_);
        target_collider->SetPos(target_pos_);
        hit = true;
        return;
    }
    //std::cout<<"target_pos_ is invalid"<<std::endl;
    //try moving to target
    while ((GetPos() - target_pos_).Distance() > 0.1f && !hit)
    {
        Vector2 dir = (target_pos_ - GetPos()).Normalize();
        Vector2 pos = GetPos() + dir * GameObject::SIZE_ / 2;
        SetPos(pos);
        //Get collision
        Collider* collider = GetCollider();
        collider->Update();
        GameBase::GetCollMngr()->UpdateCollision(collider);
        std::list<Collider*> collision = collider->GetCollision();
        if (collision.size() == 0)
        {
            continue;
        }
        else if (collision.size())
        {
            collision.sort([collider](Collider* a, Collider* b)
            {
                return (a->GetPos() - collider->GetPos()).Distance() < (b->GetPos() - collider->GetPos()).Distance();
            });
            for (auto& col : collision)
            {
                if (col->GetParent()->GetType() == OBJ_SOLID)
                {
                    hit = true;
                    collider->CollisionSolid(col);
                }
            }
        }
    }
    SetPos(GetCollider()->GetPos());
}



DarkEffect::DarkEffect(Dark* parent)
    :MovableObj(parent->GetPlayer()->GetPos(), 0.0f, LoadTexture(Asset::GetAsset(dark_move_charge)), Vector2::Zero),
    parent_(parent), move_time_(0.0f), teleport_(false), charge_(false)
{
    GetCollider()->Discard();
    SetCollider(nullptr);

    SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));
    SetType(OBJ_VOID);
    SetColor(Color(0, 0, 0, 0));
    GetAnimator()->SetDrawPriority(25);
}

void DarkEffect::Update()
{
    //moveに遷移したタイミングではポジションがまだ決まっていないためポジションが決まった後にセットする
    if (move_time_ < 0.1f)
    {
        Vector2 pos = parent_->GetPlayer()->GetPos();
        SetPos(pos);
    }

    if (teleport_)
    {
        move_time_ += Time::GetDeltaTime();
        if (move_time_ > 1.0f / 2)
        {
            SetColor(Color(0, 0, 0, 0));
            GetAnimator()->SetIsAnim(false);

            teleport_ = false;
            move_time_ = 0.0f;
        }
    }

    if (!charge_ && !teleport_)
    {
        Idle();
    }
}

void DarkEffect::Idle()
{
    SetTexNo(LoadTexture(Asset::GetAsset(dark_idle)));
    SetColor(Color(1, 1, 1, 1));
    GetAnimator()->SetTexenum(dark_idle);
    GetAnimator()->SetLoopAnim(DARK_IDLE_ANIM);
    GetAnimator()->SetDrawPriority(25);

    Vector2 pos = parent_->GetPlayer()->GetPos();
    pos.y = pos.y - parent_->GetPlayer()->GetScale().y / 2;
    SetPos(pos);
}

void DarkEffect::Move()
{
    SetTexNo(LoadTexture(Asset::GetAsset(dark_move)));
    SetColor(Color(1, 1, 1, 1));
    GetAnimator()->SetTexenum(dark_move);
    GetAnimator()->SetLoopAnim(DARK_MOVE_ANIM);
    GetAnimator()->SetDrawPriority(75);
    charge_ = false;
    teleport_ = true;
}

void DarkEffect::Charge()
{
    SetTexNo(LoadTexture(Asset::GetAsset(dark_move_charge)));
    SetColor(Color(1, 1, 1, 1));
    GetAnimator()->SetTexenum(dark_move_charge);
    GetAnimator()->SetLoopAnim(DARK_MOVE_CHARGE_ANIM);
    GetAnimator()->SetDrawPriority(75);
    charge_ = true;

    Vector2 pos = parent_->GetPlayer()->GetPos();
    SetPos(pos);

    parent_->GetPlayer()->GetAnimator()->SetLoopAnim(PLAYER_FD_MOVE_ANIM);
}

DarkAttackIndicator::DarkAttackIndicator()
    : MovableObj(Vector2::Zero, 0.0f, LoadTexture(Asset::GetAsset(effect_water_atk_indicator)), Vector2::Zero)
{
    GetCollider()->Discard();
    SetCollider(nullptr);

    SetType(OBJ_VOID);
    SetColor(Color(1, 1, 1, 1));
    GetAnimator()->SetDrawPriority(25);
    GetAnimator()->SetTexenum(effect_water_atk_indicator);
    GetAnimator()->SetLoopAnim(EFFECT_WATER_ATK_INDICATOR);
}
