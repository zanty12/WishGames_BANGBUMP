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
    bool isDraw = false;

    if (responseMinStickDistance < stick.Distance()) {
        Vector2 direction = stick * speed;
        auto enemies = player_->GetMapMngr()->GetEnemyMngr()->GetEnemies();
        attackCollider = Vertex4(player_->GetPos(), player_->GetPos() + direction * attackInjectionLength, attackWidthLength);

        isDraw = true;
        
        for (auto enemy : enemies) {
            if (enemy) {
                Vertex4 enemyCollider(enemy->GetPos(), 0.0f, enemy->GetScale());

                if (Collider2D::Touch(attackCollider, enemyCollider)) {
                    enemy->Die();
                }
            }
        }
    }
}
void Fire::Draw(Vector2 offset) {
    DrawCollider(attackCollider, Color::Green, offset);
}

