#include "prep.h"

#include "sound.h"
#include "xinput.h"

std::map<ATTRIBUTE_ACTION, std::string> video_file_map =
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


std::list<attribute_select> move_list;
std::list<attribute_select> attack_list;

constexpr float x_radius = 300.0f;
constexpr float y_radius = 80.0f;

Prep::Prep(SceneMngr* scene_mngr) : scene_mngr_(scene_mngr)
{
    SetNewVideo(move_);
    //通常テキスチャー
    character_ = LoadTexture("data/texture/UI/practice_prep/player1_base.png");
    move_list = {
        {
            FIRE_MOVE, 1, (1 + 2) % 4 * M_PI_2, (1 + 2) % 4 * M_PI_2,
            LoadTexture("data/texture/UI/practice_prep/player_boot1.png"),
            Vector2(540 + cos((1 + 2) % 4 * M_PI_2) * x_radius,
                    1080 / 2 + (sin(((1 + 2) % 4) * M_PI_2) + 1) * y_radius),
            Vector2(540 + cos(((1 + 2) % 4) * M_PI_2) * x_radius,
                    1080 / 2 + (sin(((1 + 2) % 4) * M_PI_2) + 1) * y_radius)
        },
        {
            WIND_MOVE, 2, (2 + 2) % 4 * M_PI_2, (2 + 2) % 4 * M_PI_2,
            LoadTexture("data/texture/UI/practice_prep/player_boot4.png"),
            Vector2(540 + cos((2 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((2 + 2) % 4 * M_PI_2) + 1) * y_radius),
            Vector2(540 + cos((2 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((2 + 2) % 4 * M_PI_2) + 1) * y_radius)
        },
        {
            THUNDER_MOVE, 3, (3 + 2) % 4 * M_PI_2, (3 + 2) % 4 * M_PI_2,
            LoadTexture("data/texture/UI/practice_prep/player_boot3.png"),
            Vector2(540 + cos((3 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((3 + 2) % 4 * M_PI_2) + 1) * y_radius),
            Vector2(540 + cos((3 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((3 + 2) % 4 * M_PI_2) + 1) * y_radius)
        },
        {
            DARK_MOVE, 4, (4 + 2) % 4 * M_PI_2, (4 + 2) % 4 * M_PI_2,
            LoadTexture("data/texture/UI/practice_prep/player_boot2.png"),
            Vector2(540 + cos((4 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((4 + 2) % 4 * M_PI_2) + 1) * y_radius),
            Vector2(540 + cos((4 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((4 + 2) % 4 * M_PI_2) + 1) * y_radius)
        }
    };
    attack_list = {
        {
            FIRE_ATTACK, 1, (1 + 2) % 4 * M_PI_2, (1 + 2) % 4 * M_PI_2,
            LoadTexture("data/texture/UI/practice_prep/player_hand1.png"),
            Vector2(540 + cos((1 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((1 + 2) % 4 * M_PI_2) + 1) * y_radius),
            Vector2(540 + cos((1 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((1 + 2) % 4 * M_PI_2) + 1) * y_radius)
        },
        {
            WIND_ATTACK, 2, (2 + 2) % 4 * M_PI_2, (2 + 2) % 4 * M_PI_2,
            LoadTexture("data/texture/UI/practice_prep/player_hand4.png"),
            Vector2(540 + cos((2 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((2 + 2) % 4 * M_PI_2) + 1) * y_radius),
            Vector2(540 + cos((2 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((2 + 2) % 4 * M_PI_2) + 1) * y_radius)
        },
        {
            THUNDER_ATTACK, 3, (3 + 2) % 4 * M_PI_2, (3 + 2) % 4 * M_PI_2,
            LoadTexture("data/texture/UI/practice_prep/player_hand3.png"),
            Vector2(540 + cos((3 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((3 + 2) % 4 * M_PI_2) + 1) * y_radius),
            Vector2(540 + cos((3 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((3 + 2) % 4 * M_PI_2) + 1) * y_radius)
        },
        {
            DARK_ATTACK, 4, (4 + 2) % 4 * M_PI_2, (4 + 2) % 4 * M_PI_2,
            LoadTexture("data/texture/UI/practice_prep/player_hand2.png"),
            Vector2(540 + cos((4 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((4 + 2) % 4 * M_PI_2) + 1) * y_radius),
            Vector2(540 + cos((4 + 2) % 4 * M_PI_2) * x_radius, 1080 / 2 + (sin((4 + 2) % 4 * M_PI_2) + 1) * y_radius)
        }
    };

    //uiテキスチャー
    tex_bg_ = LoadTexture("data/texture/UI/practice_prep/prep_bg.png");
    tex_select_arrow_ = LoadTexture("data/texture/UI/UI_elemental_select.png");
    return_tex_ = LoadTexture("data/texture/UI/back.png");
    b_tex_ = LoadTexture("data/texture/UI/b.png");
    confirm_tex_ = LoadTexture("data/texture/UI/go.png");
    a_tex_ = LoadTexture("data/texture/UI/a.png");
    select_tex_ = LoadTexture((char*)"data/TEXTURE/UI/select.png");
    stick_tex_ = LoadTexture((char*)"data/TEXTURE/UI/stick.png");


    //sound
    select_se_= LoadSound("data/sound/se/attribute_select.wav");
    back_se_ = LoadSound("data/sound/se/back.wav");
}

void Prep::Update()
{
    //キー入力
    if (Input::GetStickLeft(0).x < -0.8)
    {
        clockwise_ = true;
        if (is_move_ && !move_moving_)
        {
            move_moving_ = true;
            PlaySound(select_se_,0);
            move_next_ = move_list.back().attribute;
        }
        else if (!is_move_ && !attack_moving_)
        {
            PlaySound(select_se_,0);
            attack_next_ = attack_list.back().attribute;
            attack_moving_ = true;
        }
    }
    if (Input::GetStickLeft(0).x > 0.8)
    {
        clockwise_ = false;
        if (is_move_ && !move_moving_)
        {
            move_moving_ = true;
            PlaySound(select_se_,0);
            std::list<attribute_select>::iterator it = move_list.begin();
            ++it;
            move_next_ = (*it).attribute;
        }
        else if (!is_move_ && !attack_moving_)
        {
            PlaySound(select_se_,0);
            std::list<attribute_select>::iterator it = attack_list.begin();
            ++it;
            attack_next_ = (*it).attribute;
            attack_moving_ = true;
        }
    }
    if (Input::GetStickLeft(0).y > 0.9)
    {
        if (is_move_)
        {
            is_move_ = false;
            delete video_;
            SetNewVideo(attack_);
        }
    }

    if (Input::GetStickLeft(0).y < -0.9)
    {
        if (!is_move_)
        {
            is_move_ = true;
            delete video_;
            SetNewVideo(move_);
        }
    }

    //切り替え
    if (move_ != move_next_)
    {
        move_ = move_next_;
        while (move_list.front().attribute != move_)
        {
            for (auto& move : move_list)
            {
                move.order -= 1;
                if (move.order == 0)
                {
                    move.order = 4;
                }
                move.target_rot = (move.order + 2) % 4 * M_PI_2;
            }
            move_list.push_back(move_list.front());
            move_list.pop_front();
        }

        if (is_move_)
        {
            delete video_;
            move_ = move_next_;
            SetNewVideo(move_);
        }
    }
    if (attack_ != attack_next_)
    {
        attack_ = attack_next_;
        while (attack_list.front().attribute != attack_)
        {
            for (auto& attack : attack_list)
            {
                attack.order -= 1;
                if (attack.order == 0)
                {
                    attack.order = 4;
                }
                attack.target_rot = (attack.order + 2) % 4 * M_PI_2;
            }
            attack_list.push_back(attack_list.front());
            attack_list.pop_front();
        }

        if (!is_move_)
        {
            delete video_;
            attack_ = attack_next_;
            SetNewVideo(attack_);
        }
    }

    //lerp each attribute to target position
    for (auto& move : move_list)
    {
        if (move.rot < move.target_rot - 0.02f || move.rot > move.target_rot + 0.02f)
        {
            move_moving_ = true;
            if (clockwise_)
            {
                move.rot += M_PI_2 * Time::GetDeltaTime();
                if (move.rot > 2 * M_PI)
                {
                    move.rot = 0;
                }
            }
            else
            {
                move.rot -= M_PI_2 * Time::GetDeltaTime();
                if (move.rot < 0)
                {
                    move.rot = 2 * M_PI;
                }
            }
            move.pos = Vector2(540 + cos(move.rot) * x_radius, 1080 / 2 + (sin(move.rot) + 1) * y_radius);
        }
        else
        {
            move_moving_ = false;
        }
    }
    for (auto& attack : attack_list)
    {
        if (attack.rot < attack.target_rot - 0.02f || attack.rot > attack.target_rot + 0.02f)
        {
            attack_moving_ = true;
            if (clockwise_)
            {
                attack.rot += M_PI_2 * Time::GetDeltaTime();
                if (attack.rot > 2 * M_PI)
                {
                    attack.rot = 0;
                }
            }
            else
            {
                attack.rot -= M_PI_2 * Time::GetDeltaTime();
                if (attack.rot < 0)
                {
                    attack.rot = 2 * M_PI;
                }
            }
            attack.pos = Vector2(540 + cos(attack.rot) * x_radius, 1080 / 2 + (sin(attack.rot) + 1) * y_radius);
        }
        else
        {
            attack_moving_ = false;
        }
    }

    video_->Update();
    if (Input::GetKeyDown(0, Input::A)) //Aボタン
    {
        {
            std::string message;
            switch (move_)
            {
            case FIRE_MOVE:
                message += "FIRE ";
                break;
            case WIND_MOVE:
                message += "WIND ";
                break;
            case THUNDER_MOVE:
                message += "THUNDER ";
                break;
            case DARK_MOVE:
                message += "DARK ";
                break;
            default:
                message += "FIRE ";
                break;
            }
            switch (attack_)
            {
            case FIRE_ATTACK:
                message += "FIRE ";
                break;
            case WIND_ATTACK:
                message += "WIND ";
                break;
            case THUNDER_ATTACK:
                message += "THUNDER ";
                break;
            case DARK_ATTACK:
                message += "DARK ";
                break;
            default:
                message += "FIRE ";
            }
            if (scene_mngr_) scene_mngr_->ChangeScene(SCENE_GAME, message);
        }
    }
    if (Input::GetKeyDown(0, Input::B))
    {
        PlaySound(back_se_, 0);
        scene_mngr_->ChangeScene(SCENE_MENU);
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
    auto move_it = move_list.rbegin();
    auto attack_it = attack_list.rbegin();

    for (int i = move_list.size(); i > 0; --i)
    {
        float alpha = 1.0f;
        if (is_move_ && i == 1)
            alpha = 1.0f;
        else
            alpha = 0.5f;

        DrawSprite(move_it->tex, Vector2(move_it->pos.x * scale_x, move_it->pos.y * scale_y), 0.0f,
                   Vector2(768 * scale_x, 768 * scale_y), Color(1.0f, 1.0f, 1.0f, alpha));

        if (!is_move_ && i == 1)
            alpha = 1.0f;
        else
            alpha = 0.5f;

        DrawSprite(attack_it->tex, Vector2(attack_it->pos.x * scale_x, attack_it->pos.y * scale_y), 0.0f,
                   Vector2(768 * scale_x, 768 * scale_y), Color(1.0f, 1.0f, 1.0f, alpha));

        if (i == 3)
            //キャラクター
            DrawSprite(character_, Vector2(540 * scale_x, 1080 / 2 * scale_y), 0.0f,
                       Vector2(768 * scale_x, 768 * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));

        ++move_it;
        ++attack_it;
    }

    //右下操作説明
    //戻る
    DrawSprite(return_tex_, Vector2(((Graphical::GetWidth() -100) ), 80), 0.0f,
               Vector2(80.0f , 80.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(b_tex_, Vector2(((Graphical::GetWidth() -160)), 80), 0.0f,
               Vector2(100.0f , 100.0f ), Color(1.0f, 1.0f, 1.0f, 1.0f));
    //決定
    DrawSprite(confirm_tex_, Vector2(((Graphical::GetWidth() -240) ), 80), 0.0f,
               Vector2(80.0f , 80.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(a_tex_, Vector2(((Graphical::GetWidth() -305) ), 80), 0.0f,
               Vector2(100.0f , 100.0f ), Color(1.0f, 1.0f, 1.0f, 1.0f));
    //選択
    DrawSprite(select_tex_, Vector2(((Graphical::GetWidth() -390) ), 80), 0.0f,
               Vector2(80.0f , 80.0f ), Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(stick_tex_, Vector2(((Graphical::GetWidth() -475) ), 82), 0.0f,
               Vector2(115.0f , 118.0f ), Color(1.0f, 1.0f, 1.0f, 1.0f));

    //動画
    video_->DrawAsResource();
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
            delete video_;
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
            delete video_;
            SetNewVideo(attack_);
        }
    }
    ImGui::End();
    ImGui::Begin("Game Start");
    if (ImGui::Button("Start"))
    {
        std::string message;
        switch (move_)
        {
        case FIRE_MOVE:
            message += "FIRE ";
            break;
        case WIND_MOVE:
            message += "WIND ";
            break;
        case THUNDER_MOVE:
            message += "THUNDER ";
            break;
        case DARK_MOVE:
            message += "DARK ";
            break;
        default:
            message += "FIRE ";
            break;
        }
        switch (attack_)
        {
        case FIRE_ATTACK:
            message += "FIRE ";
            break;
        case WIND_ATTACK:
            message += "WIND ";
            break;
        case THUNDER_ATTACK:
            message += "THUNDER ";
            break;
        case DARK_ATTACK:
            message += "DARK ";
            break;
        default:
            message += "FIRE ";
        }
        if (scene_mngr_) scene_mngr_->ChangeScene(SCENE_GAME, message);
    }
    ImGui::End();
}

void Prep::SetNewVideo(ATTRIBUTE_ACTION video_file)
{
    video_ = new Video(video_file_map.at(video_file).c_str());
    video_->SetSize(Vector2(853.3 * static_cast<float>(Graphical::GetWidth()) / 1920,
                            480 * static_cast<float>(Graphical::GetHeight()) / 1080));
    video_->SetLoop(true);
    video_->SetWindowPos(Vector2(1400 * static_cast<float>(Graphical::GetWidth()) / 1920,
                                 1000 / 2 * static_cast<float>(Graphical::GetHeight()) / 1080));
}
