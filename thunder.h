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
#include <list>

class Thunder : public Attribute
{
    struct Arrow {
        Vector2 position;
        Vector2 velocity;
    };

    const float responseMinStickDistance = 0.2f;    // スティック傾けたとき判定する距離
    const float maxChage = 100.0f;                  // チャージの最大値
    const float arrowGravity = 0.5f;                // 矢の重力
    const Vector2 arrowSize = Vector2(100, 10);     // 矢のサイズ→向きね
    const float movePower = 1;                      // 移動の力
    const float attackPower = 5;                    // 矢の移動の力
    float charge = 0.0f;                            // チャージの値
    float responseMinOneFrameDistance = 0.2f;       // 1フレームでのスティック移動量（弾くときの判定値）
    std::list<Arrow> arrows_;

public:

    Thunder(Player *player) : Attribute(player) {}
    bool StickTrigger();
    Vector2 Move() override;
    void Action() override;
};

