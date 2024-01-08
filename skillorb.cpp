#include "skillorb.h"

#include "asset.h"

SkillOrbMovable::SkillOrbMovable(int cellX, int cellY, SKILLORB_SIZE_DESC sizeDesc)
    : MovableObj(
        Vector2(cellX * GameObject::SIZE_ + GameObject::SIZE_ / 2,
                cellY * GameObject::SIZE_ + GameObject::SIZE_ / 2),
        0.0f,
        LoadTexture(Asset::GetAsset(skill_orb)),
        Vector2::Zero
    ), SkillOrb(sizeDesc)
{
    SetMovable(true);
    // タイプの指定
    SetType(OBJ_ITEM);
    // サイズ関連の指定
    this->SetScale(Vector2(radius_ / 2, radius_ / 2));
    this->GetAnimator()->SetScale(Vector2(radius_ / 2, radius_ / 2));
    //colliderrect updates itself
}

SkillOrbMovable::SkillOrbMovable(Vector2 pos, SKILLORB_SIZE_DESC sizeDesc)
    : MovableObj(pos, 0.0f, LoadTexture(Asset::GetAsset(skill_orb)), Vector2::Zero), SkillOrb(sizeDesc)
{
    SetMovable(true);
    // タイプの指定
    SetType(OBJ_ITEM);
    // サイズ関連の指定
    this->SetScale(Vector2(radius_ / 2, radius_ / 2));
    this->GetAnimator()->SetScale(Vector2(radius_ / 2, radius_ / 2));
    //colliderrect updates itself
}

void SkillOrbMovable::Update()
{
}

SkillOrbStatic::SkillOrbStatic(int cellX, int cellY, SKILLORB_SIZE_DESC desc) : GameObject(
    Vector2(cellX * GameObject::SIZE_ + GameObject::SIZE_ / 2,
            cellY * GameObject::SIZE_ + GameObject::SIZE_ / 2),
    0.0f,
    LoadTexture(Asset::GetAsset(skill_orb))), SkillOrb(desc)
{
    // タイプの指定
    SetType(OBJ_ITEM);
    // サイズ関連の指定
    this->SetScale(Vector2(radius_ / 2, radius_ / 2));
    this->GetAnimator()->SetScale(Vector2(radius_ / 2, radius_ / 2));
    //colliderrect updates itself
}

SkillOrbStatic::SkillOrbStatic(Vector2 pos, SKILLORB_SIZE_DESC sizeDesc) : GameObject(
    pos, 0.0, LoadTexture(Asset::GetAsset(skill_orb))), SkillOrb(sizeDesc)
{
    // タイプの指定
    SetType(OBJ_ITEM);

    // サイズ関連の指定
    this->SetScale(Vector2(radius_ / 2, radius_ / 2));
    this->GetAnimator()->SetScale(Vector2(radius_ / 2, radius_ / 2));
    //colliderrect updates itself
}

void SkillOrbStatic::Update()
{
}

