#include "Boss.h"
#include "bossatk.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

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
    //HP��0�ɂȂ��������
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

    if (time_ > 0.5f)
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
    int boosrand = rand() % 100;

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

    }
}

void Boss::Fire()
{
    //3���̂P�b�ɂP�q�b�g�@�T�O�_���[�W
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
    //�P�q�b�g�@�Q�O�O�_���[�W
    
    SetAtk(200);

}

void Boss::Wind()
{
    //�S���̂P�b�ɂP�q�b�g�@�R�O�_���[�W
    atk_time_ += Time::GetDeltaTime();
    if (atk_time_ > (1.0f / 4))
    {
        atk_time_ = 0;
        SetAtk(30);
    }
}

void Boss::Water()
{
    //�Q���̂P�b�ɂP�q�b�g�@�R�O�_���[�W
    atk_time_ += Time::GetDeltaTime();
    if (atk_time_ > (1.0f / 2))
    {
        atk_time_ = 0;
        SetAtk(30);
    }
    
}


