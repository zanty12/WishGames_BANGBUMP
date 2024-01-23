#include "boss.h"
#include "bossatk.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"
#include <random>
#include "scene.h"

Boss::Boss(int x, int y, EnemyMngr* enemy_mngr)
    : Enemy(x, y, LoadTexture("data/texture/boss.png"), enemy_mngr)
{
    startPosition = GetPos();
    SetScale(Vector2(SIZE_ * 6, SIZE_ * 6));
    SetHp(300);
    time_ = 0;
    atk_time_ = 0;
    atk_now = false;
    srand((unsigned int)time(NULL));
}

void Boss::Update()
{
    //HPÇ™0Ç…Ç»Ç¡ÇΩÇÁè¡Ç∑,ÉäÉUÉãÉgÇ÷
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Discard();
        GetEnemyMngr()->GetMapMngr()->GetGame()->SetChangeScene(1);
    }

    time_ += Time::GetDeltaTime();

    if (time_ > 5.0f)
    {
        if (atk_now == false)
        {
            atk_now = true;
        }
        else
        {
            atk_now = false;
        }
        time_ = 0;


        if (atk_now == true)
        {
            Atk();
            atk_now = false;
        }
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
    std::random_device rd;
    std::uniform_int_distribution<> dist(0, 100);
    int boosrand = dist(rd);

    if (boosrand < 10)
    {
        Thunder();
    }
    else if (boosrand > 10 && boosrand < 30)
    {
        Fire();
    }
    else if (boosrand > 30 && boosrand < 60)
    {
        Wind();
    }
    else if (boosrand > 60 && boosrand < 100)
    {
        Water();
    }



}

void Boss::Fire()
{
    //3ï™ÇÃÇPïbÇ…ÇPÉqÉbÉgÅ@ÇTÇOÉ_ÉÅÅ[ÉW
    Boss_Fire* fire = new Boss_Fire(startPosition);
    atk_time_ += Time::GetDeltaTime();
    GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(fire);
    if (atk_time_ > (1.0f / 3))
    {
        atk_time_ = 0;
        SetAtk(50);
    }
}

void Boss::Thunder()
{
    //ÇPÉqÉbÉgÅ@ÇQÇOÇOÉ_ÉÅÅ[ÉW
    atk_time_ ++;

    if (atk_time_ > 0 && atk_time_ < 2)
    {
        Boss_Thunder* thunder = new Boss_Thunder(startPosition);
        GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(thunder);
        SetAtk(200);
    }
    else if (atk_time_ > 2 && atk_time_ < 4)
    {
        Boss_Thunder* thunder1 = new Boss_Thunder(startPosition);
        GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(thunder1);
        thunder1->SetVel(Vector2(0.0f, thunder1->GetVel().y));
        SetAtk(200);
    }
    else if (atk_time_ > 4 && atk_time_ < 6)
    {
        Boss_Thunder* thunder2 = new Boss_Thunder(startPosition);
        GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(thunder2);
        thunder2->SetVel(Vector2(thunder2->GetVel().x * -1, thunder2->GetVel().y));
        SetAtk(200);
    }

    if (atk_time_ > 6)
    {
        atk_time_ = 0;
    }
    

}

void Boss::Wind()
{
    //ÇSï™ÇÃÇPïbÇ…ÇPÉqÉbÉgÅ@ÇRÇOÉ_ÉÅÅ[ÉW
    Boss_Wind* wind = new Boss_Wind(startPosition);
    atk_time_ += Time::GetDeltaTime();
    GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(wind);
    
        if (atk_time_ > (1.0f / 4))
        {
            atk_time_ = 0;
            SetAtk(30);
        }
}

void Boss::Water()
{
    //ÇQï™ÇÃÇPïbÇ…ÇPÉqÉbÉgÅ@ÇRÇOÉ_ÉÅÅ[ÉW
    Boss_Water* water = new Boss_Water(startPosition);
    Boss_Water* water1 = new Boss_Water(startPosition - (Vector2(SIZE_ * 6, 0.0f)));
    Boss_Water* water2 = new Boss_Water(startPosition + (Vector2(SIZE_ * 6, 0.0f)));
    atk_time_ += Time::GetDeltaTime();
    GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(water);
    GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(water1);
    GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(water2);
    if (atk_time_ > (1.0f / 2))
    {
        atk_time_ = 0;
        SetAtk(30);
    }
    
}

