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

class Thunder : public Attribute
{
    const float responseMinStickDistance = 0.2f;    // スティック傾けたとき判定する距離
    const float maxChage = 100.0f;       // チャージの最大値
    float charge = 0.0f;                // チャージの値
    float responseMinOneFrameDistance = 0.2f;       // 1フレームでのスティック移動量（弾くときの判定値）


public:

    Thunder(Player *player) : Attribute(player) {}
    Vector2 Move() override;
    void Action() override;

};

