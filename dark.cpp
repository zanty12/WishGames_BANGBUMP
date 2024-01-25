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


Vector2 Dark::Move()
{
    Vector2 velocity = player_->GetVel();
    Vector2 stick = Input::GetStickLeft(0);

    if (stick != Vector2::Zero)
    {
        if (move_indicator_ == nullptr)
        {
            move_indicator_ = new DarkIndicator();
        }
        Vector2 dir = stick.Normalize();
        dir.y *= -1;
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
        if (velocity.y <= -maxSpeedFalling)
        {
            velocity.y = maxSpeedFalling;
            return velocity;
        }
    }
    if (Input::GetKeyUp(0, Input::LThumb))
    {
        Vector2 stick = Input::GetStickLeft(0);
        stick = stick.Normalize();
        stick.y *= -1;
        Vector2 warpDirection = stick * warpDistance_;
        warpPosition = move_indicator_->GetPos();

        player_->SetPos(warpPosition);
        delete move_indicator_;
        move_indicator_ = nullptr;
    }
    else warpPosition = Vector2::Zero;

    return player_->GetVel();
};

void Dark::Action()
{
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);
    //float distance = stick.Distance();
    if (stick != Vector2::Zero)
    {
        if (attack_indicator_ == nullptr)
        {
            attack_indicator_ = new ThunderIndicator();
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


    // 押し込む
    if (Input::GetKeyDown(0, Input::RThumb) && responseMinStickDistance < stick.Distance())
    {
        attackDirection_ = stick.Normalize();
    }
    // 攻撃
    else if (Input::GetKey(0, Input::RThumb))
    {
        if (attack_ == nullptr)
            attack_ = new DarkAttack(this);
        float angle = atan2(attackDirection_.y, attackDirection_.x);
        Vector2 pos = Vector2(cos(angle), -sin(angle)) * (player_->GetScale().x / 2 + attack_->GetScale().x / 2);
        pos = player_->GetPos() + pos;
        attack_->SetPos(pos);
        attack_->SetRot(angle);
    }
    else
    {
        if (attack_ != nullptr)
        {
            delete attack_;
            attack_ = nullptr;
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

DarkAttack::DarkAttack(Dark* parent) : parent_(parent),
                                       MovableObj(
                                           Vector2(
                                               parent->GetPlayer()->GetPos().x + parent->GetPlayer()->GetScale().x / 2 +
                                               5 * GameObject::SIZE_, parent->GetPlayer()->GetPos().y), 0.0f,
                                           LoadTexture(Asset::GetAsset(water_attack)), Vector2::Zero),PlayerAttack(10000)
{
    SetScale(size_);
    SetType(OBJ_ATTACK);
}

void DarkAttack::Update()
{
    UpdateTick();
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
                    if (GetTick() > GetMaxTick())
                    {
                        SetTick(0.0f);
                        enemy->SetHp(enemy->GetHp() - GetDamage());
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

DarkIndicator::DarkIndicator() : MovableObj(Vector2::Zero, 0.0f, LoadTexture(Asset::GetAsset(player)), Vector2::Zero)
{
    SetScale(Vector2(2 * GameObject::SIZE_, 2 * GameObject::SIZE_));
    GetAnimator()->SetColor(Color(0.0f, 0.0f, 0.5f, 0.5f));
    SetType(OBJ_VOID);
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
