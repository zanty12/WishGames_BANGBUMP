#include"fire.h"
#include"xinput.h"
#include"lib/collider2d.h"

Vector2 Fire::Move(){
    Vector2 vel = player_.GetVel();
    Vector2 stick = Input::GetStickLeft(0);
    Vector2 preStick = Input::GetPreviousStickLeft(0);
    Vector2 s = Input::GetStickLeft(0);
    player_.AddVel(-s);
};

void Fire::Action() {
    Vector2 stick = Input::GetStickRight(0);

    using namespace PHYSICS;

    Vertex4 square(Vector2(0, 1), Vector2(1, 1), Vector2(1, -1), Vector2(0, -1));
        //Vertex1=円　Vertex2=線（・と・）　Vertex3=三角形　VertexN=四角形

    Vertex1 enemy(Vector2(0, 0),2.0f);

    bool isTouch = Collider2D::Touch(enemy, square);

};

