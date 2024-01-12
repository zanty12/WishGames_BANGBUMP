#include "boss.h"
#include "bossatk.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"
#include <random>

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
    //HPÇ™0Ç…Ç»Ç¡ÇΩÇÁè¡Ç∑
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Discard();
    }

    time_ += Time::GetDeltaTime();

    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
    }

    if (time_ > 3.0f)
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
        atk_now = false;
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
    /*int boosrand = rand() % 100;
    if (boosrand < 10)
    {

    }
    else if (boosrand > 10 && boosrand < 30)
    {
        Fire();
    }
    else if (boosrand > 30 && boosrand < 60)
    {

    }
    else if (boosrand > 60 && boosrand < 100)
    {

    }*/

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
    Boss_Thunder* thunder = new Boss_Thunder(startPosition);
    atk_time_ += Time::GetDeltaTime();
    GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(thunder);
    float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //èâä˙âªéûÇÃÉGÉâÅ[ÇâÒîÇ∑ÇÈ
    float speed_ = 96.0f * 6;
    thunder->SetVel(Vector2(-speed_ * dt, -speed_ * dt));
    
    SetAtk(200);

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


