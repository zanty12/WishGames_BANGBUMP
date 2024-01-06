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

    time_ += Time::GetDeltaTime();

    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
    }

    if (time_ > 5.0f)
    {
        time_ = 0;
        if (atk_now == false)
        {
            atk_now = true;
        }
        else
        {
            atk_now = false;
        }
    }

    if (atk_now == true)
    {
        Atk();
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

    }
    else if (rrand > 10 && rrand < 30)
    {

    }
    else if (rrand > 30 && rrand < 60)
    {

    }
    else if (rrand > 60 && rrand <= 100)
    {

    }
}

void Boss::Fire()
{
    //3ï™ÇÃÇPïbÇ…ÇPÉqÉbÉgÅ@ÇTÇOÉ_ÉÅÅ[ÉW
    atk_time_ += Time::GetDeltaTime();
    if (atk_time_ > (1.0f / 3))
    {
        atk_time_ = 0;
        SetAtk(50);
    }


}

void Boss::Thunder()
{
    //ÇPÉqÉbÉgÅ@ÇQÇOÇOÉ_ÉÅÅ[ÉW
    
    SetAtk(200);

}

void Boss::Wind()
{
    //ÇSï™ÇÃÇPïbÇ…ÇPÉqÉbÉgÅ@ÇRÇOÉ_ÉÅÅ[ÉW
    atk_time_ += Time::GetDeltaTime();
    if (atk_time_ > (1.0f / 4))
    {
        atk_time_ = 0;
        SetAtk(30);
    }
}

void Boss::Water()
{
    //ÇQï™ÇÃÇPïbÇ…ÇPÉqÉbÉgÅ@ÇRÇOÉ_ÉÅÅ[ÉW
    atk_time_ += Time::GetDeltaTime();
    if (atk_time_ > (1.0f / 2))
    {
        atk_time_ = 0;
        SetAtk(30);
    }
    
}


