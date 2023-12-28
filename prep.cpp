#include "prep.h"

std::map<VIDEO_FILE, std::string> video_file_map =
{
    {FIRE_MOVE, "./data/video/fire_move.mp4"},
    {FIRE_ATTACK, "./data/video/fire_attack.mp4"},
    {WIND_MOVE, "./data/video/wind_move.mp4"},
    {WIND_ATTACK, "./data/video/wind_attack.mp4"},
    {THUNDER_MOVE, "./data/video/thunder_move.mp4"},
    {THUNDER_ATTACK, "./data/video/thunder_attack.mp4"},
    {DARK_MOVE, "./data/video/dark_move.mp4"},
    {DARK_ATTACK, "./data/video/dark_attack.mp4"},
};

std::map<int, float> rot_pos =
{
    {1, 3 * M_PI / 2},
    {2, 0},
    {3, M_PI / 2},
    {4, M_PI},
};

std::list<VIDEO_FILE> pos_move =
{
    FIRE_MOVE,
    WIND_MOVE,
    THUNDER_MOVE,
    DARK_MOVE
};

std::list<VIDEO_FILE> pos_attack =
{
    FIRE_ATTACK,
    WIND_ATTACK,
    THUNDER_ATTACK,
    DARK_ATTACK
};

constexpr float x_radius = 300.0f;
constexpr float y_radius = 80.0f;

Prep::Prep(SceneMngr* scene_mngr) : scene_mngr_(scene_mngr)
{
    SetNewVideo(move_);
    SetNewVideo(attack_);
    //通常テキスチャー
    character_ = LoadTexture("data/texture/UI/player.png");
    tex_fire_move_ = LoadTexture("data/texture/UI/fire_move.png");
    tex_fire_attack_ = LoadTexture("data/texture/UI/fire_attack.png");
    tex_wind_move_ = LoadTexture("data/texture/UI/wind_move.png");
    tex_wind_attack_ = LoadTexture("data/texture/UI/wind_attack.png");
    tex_thunder_move_ = LoadTexture("data/texture/UI/thunder_move.png");
    tex_thunder_attack_ = LoadTexture("data/texture/UI/thunder_attack.png");
    tex_dark_move_ = LoadTexture("data/texture/UI/dark_move.png");
    tex_dark_attack_ = LoadTexture("data/texture/UI/dark_attack.png");
    //uiテキスチャー
    tex_bg_ = LoadTexture("data/texture/UI/prep_bg.png");
    tex_select_arrow_ = LoadTexture("data/texture/UI/select_arrow.png");
}

void Prep::Update()
{
    //動画の切り替え
    if (move_ != move_next_)
    {
        delete video_list_.at(move_);
        video_list_.erase(move_);
        move_ = move_next_;
        SetNewVideo(move_);
        while (pos_move.front() != move_)
        {
            pos_move.push_back(pos_move.front());
            pos_move.pop_front();
        }
    }
    if (attack_ != attack_next_)
    {
        delete video_list_.at(attack_);
        video_list_.erase(attack_);
        attack_ = attack_next_;
        SetNewVideo(attack_);
        while (pos_attack.front() != attack_)
        {
            pos_attack.push_back(pos_attack.front());
            pos_attack.pop_front();
        }
    }
    if (is_move_)
    {
        video_list_.at(move_)->Update();
    }
    else
    {
        video_list_.at(attack_)->Update();
    }
}

void Prep::Draw()
{
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    //背景
    DrawSprite(tex_bg_, Vector2(static_cast<float>(Graphical::GetWidth()) / 2,
                                static_cast<float>(Graphical::GetHeight()) / 2), 0.0f,
               Vector2(1920 * scale_x, 1080 * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));

    //移動と攻撃選択
    auto move_it = pos_move.rbegin();
    auto attack_it = pos_attack.rbegin();

    for (int i = 4; i >= 1; --i)
    {
        int move_tex, attack_tex;

        switch (*move_it)
        {
        case FIRE_MOVE:
            move_tex = tex_fire_move_;
            break;
        case WIND_MOVE:
            move_tex = tex_wind_move_;
            break;
        case THUNDER_MOVE:
            move_tex = tex_thunder_move_;
            break;
        case DARK_MOVE:
            move_tex = tex_dark_move_;
            break;
        default:
            move_tex = tex_fire_move_;
            break;
        }

        switch (*attack_it)
        {
        case FIRE_ATTACK:
            attack_tex = tex_fire_attack_;
            break;
        case WIND_ATTACK:
            attack_tex = tex_wind_attack_;
            break;
        case THUNDER_ATTACK:
            attack_tex = tex_thunder_attack_;
            break;
        case DARK_ATTACK:
            attack_tex = tex_dark_attack_;
            break;
        default:
            attack_tex = tex_fire_attack_;
            break;
        }

        float alpha = 1.0f;
        if (is_move_ && i == 1)
            alpha = 1.0f;
        else
            alpha = 0.5f;

        DrawSprite(move_tex, Vector2(540 * scale_x + cos(rot_pos.at(i)) * x_radius,
                                     1080 / 2 * scale_y + (sin(rot_pos.at(i)) + 1) * y_radius), 0.0f,
                   Vector2(768 * scale_x, 768 * scale_y), Color(1.0f, 1.0f, 1.0f, alpha));

        if (!is_move_ && i == 1)
            alpha = 1.0f;
        else
            alpha = 0.5f;

        DrawSprite(attack_tex, Vector2(540 * scale_x + cos(rot_pos.at(i)) * x_radius,
                                       1080 / 2 * scale_y + (sin(rot_pos.at(i)) + 1) * y_radius), 0.0f,
                   Vector2(768 * scale_x, 768 * scale_y), Color(1.0f, 1.0f, 1.0f, alpha));

        if(i == 3)
            //キャラクター
                DrawSprite(character_, Vector2(540 * scale_x, 1080 / 2 * scale_y), 0.0f,
                           Vector2(768 * scale_x, 768 * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));

        ++move_it;
        ++attack_it;
    }

    //動画
    if (is_move_)
    {
        video_list_.at(move_)->Draw();
    }
    else
    {
        video_list_.at(attack_)->Draw();
    }
}

void Prep::DebugMenu()
{
    ImGui::Begin("Move Attribute");
    ImGui::RadioButton("Fire", (int*)&move_next_, FIRE_MOVE);
    ImGui::RadioButton("Wind", (int*)&move_next_, WIND_MOVE);
    ImGui::RadioButton("Thunder", (int*)&move_next_, THUNDER_MOVE);
    ImGui::RadioButton("Dark", (int*)&move_next_, DARK_MOVE);
    if (ImGui::Button("select"))
    {
        if (!is_move_)
        {
            is_move_ = true;
            delete video_list_.at(move_);
            video_list_.erase(move_);
            SetNewVideo(move_);
        }
    }
    ImGui::End();
    ImGui::Begin("Attack Attribute");
    ImGui::RadioButton("Fire", (int*)&attack_next_, FIRE_ATTACK);
    ImGui::RadioButton("Wind", (int*)&attack_next_, WIND_ATTACK);
    ImGui::RadioButton("Thunder", (int*)&attack_next_, THUNDER_ATTACK);
    ImGui::RadioButton("Dark", (int*)&attack_next_, DARK_ATTACK);
    if (ImGui::Button("select"))
    {
        if (is_move_)
        {
            is_move_ = false;
            delete video_list_.at(attack_);
            video_list_.erase(attack_);
            SetNewVideo(attack_);
        }
    }
    ImGui::End();
    ImGui::Begin("Game Start");
    if (ImGui::Button("Start"))
    {
        scene_mngr_->ChangeScene(SCENE_GAME);
    }
    ImGui::End();
}

void Prep::SetNewVideo(VIDEO_FILE video_file)
{
    video_list_.emplace(video_file, new Video(video_file_map.at(video_file).c_str()));
    video_list_.at(video_file)->SetSize(Vector2(853.3 * static_cast<float>(Graphical::GetWidth()) / 1920,
                                                480 * static_cast<float>(Graphical::GetHeight()) / 1080));
    video_list_.at(video_file)->SetLoop(true);
    video_list_.at(video_file)->SetWindowPos(Vector2(1400 * static_cast<float>(Graphical::GetWidth()) / 1920,
                                                     1000 / 2 * static_cast<float>(Graphical::GetHeight()) / 1080));
}
