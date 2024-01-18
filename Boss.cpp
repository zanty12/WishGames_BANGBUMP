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

    // ƒAƒjƒ[ƒ^[Ý’è
    SetTexenum(boss_idle);
    GetAnimator()->SetTexenum(GetTexenum());
    GetAnimator()->SetLoopAnim(BOSS_IDLE_ANIM);
}

void Boss::Update()
{
    //HP‚ª0<< ‚É‚È‚Á‚½‚çÁ‚·
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Die();
    }
    if (GetDiscard())
    {
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

    }



}

void Boss::Fire()
{
    //3•ª‚Ì‚P•b‚É‚Pƒqƒbƒg@‚T‚Oƒ_ƒ[ƒW
    Boss_Fire* fire = new Boss_Fire(GetPos());
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
    //‚Pƒqƒbƒg@‚Q‚O‚Oƒ_ƒ[ƒW
    
    SetAtk(200);

}

void Boss::Wind()
{
    //‚S•ª‚Ì‚P•b‚É‚Pƒqƒbƒg@‚R‚Oƒ_ƒ[ƒW
    Boss_Wind* wind = new Boss_Wind(GetPos());
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
    //‚Q•ª‚Ì‚P•b‚É‚Pƒqƒbƒg@‚R‚Oƒ_ƒ[ƒW
    atk_time_ += Time::GetDeltaTime();
    if (atk_time_ > (1.0f / 2))
    {
        atk_time_ = 0;
        SetAtk(30);
    }
    
}


