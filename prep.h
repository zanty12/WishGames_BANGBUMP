/** 準備シーン
 * プレイヤーの属性を選択するシーン
 *
 * Author: Cheung To Hung Date: 2023/12/23
 */

#pragma once
#include "scene.h"
#include "player.h"
#include "video.h"

enum VIDEO_FILE
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



class SceneMngr;
class Prep : public Scene
{
private:
    //プレイヤーの属性
    VIDEO_FILE move_ = FIRE_MOVE;
    VIDEO_FILE attack_ = FIRE_ATTACK;
    VIDEO_FILE move_next_ = FIRE_MOVE;
    VIDEO_FILE attack_next_ = FIRE_ATTACK;
    //仕様上二つしかないけど、一応
    std::map<VIDEO_FILE, Video*> video_list_;

    SceneMngr* scene_mngr_;

public:
    Prep() = delete;
    Prep(SceneMngr* scene_mngr);

    ~Prep() override
    {
        for (auto& video : video_list_)
        {
            delete video.second;
        }
    }

    void Update() override;
    void Draw() override;
    void DebugMenu() override;
};
