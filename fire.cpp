#include"fire.h"
#include"xinput.h"
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
    Vector2 stick = Input::GetStickRight(0);

    using namespace PHYSICS;

    Vertex4 square(Vector2(0, 1), Vector2(1, 1), Vector2(1, -1), Vector2(0, -1));
    //Vertex1=�~�@Vertex2=���i�E�ƁE�j�@Vertex3=�O�p�`�@VertexN=�l�p�`

    Vertex1 enemy(Vector2(0, 0), 2.0f);

    bool isTouch = Collider2D::Touch(enemy, square);

};

