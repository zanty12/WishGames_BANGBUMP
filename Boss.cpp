#include "Boss.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

void Boss::Update()
{
    //HPÇ™0Ç…Ç»Ç¡ÇΩÇÁè¡Ç∑
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Die();
    }

    Atk();

    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
    }

    GetAnimator()->SetIsAnim(true);



}

SkillOrb* Boss::DropSkillOrb()
{
    if (GetDiscard() == false)
        return nullptr;
}

void Boss::Atk()
{
    int rrand = rand() % 100;

    if (rrand < 10)
    {
        SetAtk(200);
    }
    else if (rrand > 10 && rrand < 30)
    {
        SetAtk(50);
    }
    else if (rrand > 30 && rrand < 60)
    {
        SetAtk(30);
    }
    else if (rrand > 60 && rrand <= 100)
    {
        SetAtk(30);
    }
}

void Boss::Fire()
{
}

void Boss::Thunder()
{
}

void Boss::Wind()
{


}

void Boss::Water()
{
}


