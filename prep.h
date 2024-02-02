/** 準備シーン
 * プレイヤーの属性を選択するシーン
 *
 * Author: Cheung To Hung Date: 2023/12/23
 */

#pragma once
#include "scene.h"
#include "player.h"
#include "video.h"

enum ATTRIBUTE_ACTION
{
    FIRE_MOVE,
    FIRE_ATTACK,
    WIND_MOVE,
    WIND_ATTACK,
    THUNDER_MOVE,
    THUNDER_ATTACK,
    DARK_MOVE,
    DARK_ATTACK,
};

struct attribute_select
{
    ATTRIBUTE_ACTION attribute;
    int order;
    float rot;
    float target_rot;
    int tex;
    Vector2 pos;
    Vector2 target_pos;
};

class SceneMngr;
class Prep : public Scene
{
private:
    //プレイヤーの属性
    ATTRIBUTE_ACTION move_ = FIRE_MOVE;
    ATTRIBUTE_ACTION attack_ = FIRE_ATTACK;
    ATTRIBUTE_ACTION move_next_ = FIRE_MOVE;
    ATTRIBUTE_ACTION attack_next_ = FIRE_ATTACK;
    Video *video_;

    bool clockwise_ = true;
    bool moving_ = false;

    SceneMngr *scene_mngr_;

    bool is_move_ = true;

    //通常テキスチャー
    int character_;
    int tex_fire_move_;
    int tex_fire_attack_;
    int tex_wind_move_;
    int tex_wind_attack_;
    int tex_thunder_move_;
    int tex_thunder_attack_;
    int tex_dark_move_;
    int tex_dark_attack_;
    //uiテキスチャー
    int tex_bg_;
    int tex_select_arrow_;

public:
    Prep() = delete;
    Prep(SceneMngr *scene_mngr);

    ~Prep() override
    {
        delete video_;
    }

    void Update() override;
    void Draw() override;
    void DebugMenu() override;
    ATTRIBUTE_ACTION GetMove(void) { return move_; }
    ATTRIBUTE_ACTION GetAttack(void) { return attack_; }

private:
    void SetNewVideo(ATTRIBUTE_ACTION video_file);
};
