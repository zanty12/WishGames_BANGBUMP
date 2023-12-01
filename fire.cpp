#include "fire.h"
#include "xinput.h"
#include "sprite.h"
#include"lib/collider2d.h"

Vector2 Fire::Move() {
    Vector2 stick = Input::GetStickLeft(0);
    stick.y *= -1;

    if (responseMinStickDistance < stick.Distance()) {
        Vector2 direction = -stick * speed;
        return direction;
    }
    else {
        Vector2 direction = player_->GetVel() *= friction;
        return direction;
    }
};

void Fire::Action() {
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);
    float distance = stick.Distance();

    //Vertex1=円　Vertex2=線（・と・）　Vertex3=三角形　VertexN=四角形


    if (responseMinStickDistance < stick.Distance()) {
        Vector2 direction = -stick * speed;
        //auto enemies = player_->GetMapMngr()->GetEnemyMngr()->GetEnemies();
        Vertex4 attackCollider(player_->GetPos(), direction * attackInjectionLength, 100);

        float linewidth = 0.1f;
        DrawLine(attackCollider.a, attackCollider.b, Color::Green, linewidth);
        DrawLine(attackCollider.b, attackCollider.c, Color::Green, linewidth);
        DrawLine(attackCollider.c, attackCollider.d, Color::Green, linewidth);
        DrawLine(attackCollider.d, attackCollider.a, Color::Green, linewidth);

       /* for (auto enemy : enemies) {            
            Vertex4 enemyCollider(enemy->GetPos(), 0.0f, enemy->GetScale());

            if (Collider2D::Touch(attackCollider, enemyCollider)) {
                enemy->Die();
            }
        }*/
    }
    else {

    }

};

