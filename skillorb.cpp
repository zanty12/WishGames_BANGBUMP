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
    // �^�C�v�̎w��
    SetType(OBJ_ITEM);
    // �T�C�Y�֘A�̎w��
    this->SetScale(Vector2(radius_ / 2, radius_ / 2));
    this->GetAnimator()->SetScale(Vector2(radius_ / 2, radius_ / 2));
    //colliderrect updates itself
}

SkillOrbMovable::SkillOrbMovable(Vector2 pos, SKILLORB_SIZE_DESC sizeDesc)
    : MovableObj(pos, 0.0f, LoadTexture(Asset::GetAsset(skill_orb)), Vector2::Zero), SkillOrb(sizeDesc)
{
    SetMovable(true);
    // �^�C�v�̎w��
    SetType(OBJ_ITEM);
    // �T�C�Y�֘A�̎w��
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
    // �^�C�v�̎w��
    SetType(OBJ_ITEM);
    // �T�C�Y�֘A�̎w��
    this->SetScale(Vector2(radius_ / 2, radius_ / 2));
    this->GetAnimator()->SetScale(Vector2(radius_ / 2, radius_ / 2));
    //colliderrect updates itself
}

SkillOrbStatic::SkillOrbStatic(Vector2 pos, SKILLORB_SIZE_DESC sizeDesc) : GameObject(
    pos, 0.0, LoadTexture(Asset::GetAsset(skill_orb))), SkillOrb(sizeDesc)
{
    // �^�C�v�̎w��
    SetType(OBJ_ITEM);

    // �T�C�Y�֘A�̎w��
    this->SetScale(Vector2(radius_ / 2, radius_ / 2));
    this->GetAnimator()->SetScale(Vector2(radius_ / 2, radius_ / 2));
    //colliderrect updates itself
}

void SkillOrbStatic::Update()
{
}

