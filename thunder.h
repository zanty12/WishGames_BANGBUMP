//--------------------------------------------------------------------------------
// 
// サンダー[thunder.h]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
// 
//--------------------------------------------------------------------------------
#pragma once
#include "attribute.h"
#include "lib/list.h"

class Thunder : public Attribute
{
    struct Arrow {
        Vector2 position;
        Vector2 velocity;
        bool isVisible = false;
    };

    const float responseMinStickDistance = 0.2f;    // スティック傾けたとき判定する距離
    //const float maxChage = 100.0f;                  // チャージの最大値
    const float arrowGravity = 0.5f;                // 矢の重力
    const Vector2 arrowSize = Vector2(100, 10);     // 矢のサイズ→向きね
    //const float movePower = 1;                      // 移動の力
    //const float attackPower = 5;                    // 矢の移動の力
    float charge = 0.0f;                            // チャージの値
    float responseMinOneFrameDistance = 0.2f;       // 1フレームでのスティック移動量（弾くときの判定値）
    Arrow arrows_[3];

    //ここからは調整用のためconst抜き
    float maxCharge = 100.0f;
    float movePower = 1;
    float attackPower = 5;

public:
    Thunder(Player *player) : Attribute(player) {}
    ~Thunder() override = default;
    bool StickTrigger(Vector2 stick, Vector2 previousStick);
    Vector2 Move() override;
    void Action() override;
    void Draw(Vector2 offset) override;
    void DebugMenu() override;
};

