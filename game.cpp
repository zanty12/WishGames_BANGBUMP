#include "game.h"
#include "dark.h"
#include "fire.h"

#include "thunder.h"
#include "wind.h"
#include <thread>

#include "asset.h"
#include "text.h"

#include "sound.h"
#include "xinput.h"

Game::Game(SceneMngr* scene_mngr)
    : GameBase(scene_mngr)
{
    renderer_ = new Renderer();
    coll_mngr_ = new CollMngr();
    projectile_mngr_ = new ProjectileMngr();
    orb_mngr_ = new SkillOrbMngr();
    players_ = std::list<Player *>();
    mapmngr_ = new MapMngr(Asset::GetAsset(practices_stage).c_str(), this);

    text_format_ = Text::MakeTextFormat(L"ワープロ明朝", 50, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL);
    brush_ = Text::MakeBrush(Color(1.0f, 1.0f, 1.0f, 1.0f));
    timer_tex_ = LoadTexture(Asset::GetAsset(timer));
    //int playertex = LoadTexture("data/texture/player.png");

    /*Player *player_ = new Player(mapmngr_->GetPlayerSpawn(), 0.0f, Vector2(0.0f, 0.0f), mapmngr_);
    players_.push_back(player_);
    camera_ = new Camera(player_);*/

    //BGM追加、再生
    practicemode_bgm_ = LoadSound(Asset::GetAsset(BGM_practicemode).c_str());
    PlaySound(practicemode_bgm_, -1);
}


void Game::Update()
{
    if (!Input::GetKeyDown(0, Input::B) && !result_)
        UpdateNormal();
    else
        UpdateResult();
}

void Game::Draw()
{
    if (!result_)
        DrawNormal();
    else
        DrawResult();

    coll_mngr_->Draw(camera_);

}

Player* Game::GetPlayer()
{
    return *GetPlayers().begin();
}

void Game::DebugMenu()
{
    auto player_ = GetPlayer();
    player_->DebugMenu();
    ImGui::Begin("Game");
    ImGui::Text(u8"プレイヤー座標");
    ImGui::Text("x:%f, y: %f", player_->GetPos().x, player_->GetPos().y);
    ImGui::Text(u8"プレイヤースピード");
    ImGui::Text("x:%f, y: %f", player_->GetVel().x, player_->GetVel().y);
    ImGui::Text(u8"プレイヤー状態");
    ImGui::Text("%d", player_->GetPlayerState());
    ImGui::Text(u8"カメラ座標");
    ImGui::Text("x:%f, y: %f", camera_->GetPos().x, camera_->GetPos().y);
    static std::string preview = u8"無属性";
    static std::string preview_atk = u8"無属性";
    if (player_->GetAttribute() == nullptr)
        preview = u8"無属性";
    if (player_->GetAttackAttribute() == nullptr)
        preview_atk = u8"無属性";

    if (ImGui::BeginCombo(u8"プレイヤー移動属性", preview.c_str()))
    {
        if (ImGui::Selectable("Fire", false))
        {
            preview = "Fire";
            player_->SetAttribute(new Fire(player_));
        }
        if (ImGui::Selectable("Wind", false))
        {
            preview = "Wind";
            player_->SetAttribute(new Wind(player_));
        }
        if (ImGui::Selectable("Thunder", false))
        {
            preview = "Thunder";
            player_->SetAttribute(new Thunder(player_));
        }
        if (ImGui::Selectable("Dark", false))
        {
            preview = "Dark";
            player_->SetAttribute(new Dark(player_));
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo(u8"プレイヤー攻撃属性", preview_atk.c_str()))
    {
        if (ImGui::Selectable("Fire", false))
        {
            preview_atk = "Fire";
            player_->SetAttackAttribute(new Fire(player_));
        }
        if (ImGui::Selectable("Wind", false))
        {
            preview_atk = "Wind";
            player_->SetAttackAttribute(new Wind(player_));
        }
        if (ImGui::Selectable("Thunder", false))
        {
            preview_atk = "Thunder";
            player_->SetAttackAttribute(new Thunder(player_));
        }
        if (ImGui::Selectable("Dark", false))
        {
            preview_atk = "Dark";
            player_->SetAttackAttribute(new Dark(player_));
        }
        ImGui::EndCombo();
    }
    ImGui::End();
    ImGui::Begin("map test");
    if (ImGui::Button("map1"))
    {
        delete mapmngr_;
        coll_mngr_->CheckDiscard();
        renderer_->CheckDiscard();
        mapmngr_ = new MapMngr("data/map/Boss_Map.csv", this);
    }
    if (ImGui::Button("clear"))
        SetChangeScene(1);
    if (ImGui::Button("over"))
        SetChangeScene(2);
    if(ImGui::Button("title"))
    {
        scene_mngr_->ChangeScene(SCENE_TITLE);
    }
    ImGui::End();

}

void Game::UpdateNormal()
{
    if(first_update_)
    {
        //処理落ち回避のため一回目の更新をスキップする
        first_update_ = false;
        return;
    }
    if(start_timer_ > 0.0f)
    {
        coll_mngr_->Update();
        camera_->Update(GetPlayer()->GetPos(), GetPlayer()->GetVel(), GetPlayer()->GetPlayerState());
        renderer_->Update();
        start_timer_ -= Time::GetDeltaTime();
        return;
    }
    coll_mngr_->Update();
    std::thread map(&MapMngr::Update, mapmngr_);
    //mapmngr_->Update();
    
    std::thread player(&Player::Update, GetPlayer());
    //GetPlayer()->Update();
    std::thread projectile(&ProjectileMngr::Update, projectile_mngr_);
    //projectile_mngr_->Update();
    //std::thread orb(&SkillOrbMngr::Update, orb_mngr_);
    std::thread enemy(&EnemyMngr::Update, mapmngr_->GetEnemyMngr());
    //projectile.join();
    //orb.join();
    std::thread camera(&Camera::Update, camera_, GetPlayer()->GetPos(), GetPlayer()->GetVel(),
                       GetPlayer()->GetPlayerState());
    //camera_->Update(GetPlayer()->GetPos(),GetPlayer()->GetVel(),GetPlayer()->GetPlayerState());
    std::thread renderer(&Renderer::Update, renderer_);
    //renderer_->Update();
    map.join();
    player.join();
    projectile.join();
    enemy.join();
    camera.join();
    renderer.join();
    orb_mngr_->Update();
    //check scene change
    if (GetPlayer()->GetChangeSceneFlag())
    {
        change_scene_ = 2;
    }
    /*if (GetChangeScene() == 1)
    {
        scene_mngr_->ChangeScene(SCENE_RESULT);
    }*/

    //timer
    timer_ -= Time::GetDeltaTime();
    if (timer_ <= 0.0f)
    {
        delete mapmngr_;
        coll_mngr_->CheckDiscard();
        renderer_->CheckDiscard();
        mapmngr_ = new MapMngr(Asset::GetAsset(single_stage_2).c_str(), this);
        timer_ = 500.0f;
        GetPlayer()->SetPos(mapmngr_->GetPlayerSpawn());
        GetPlayer()->SetMapMngr(mapmngr_);
        delete camera_;
        camera_ = new Camera(GetPlayer()->GetPos(),
                             Vector2(mapmngr_->GetMap()->GetWidth(), mapmngr_->GetMap()->GetHeight()));
        first_update_ = true;
    }

}

void Game::DrawNormal()
{
    if(start_timer_ > 0.0f)
    {
        int itimeer = static_cast<int>(start_timer_);
        std::wstring time = std::to_wstring(itimeer);
        if(itimeer == 0)
        {
            time = L"GO!";
        }
        Text::WriteText(time.c_str(), text_format_, brush_, Graphical::GetWidth() / 2 - 45, Graphical::GetHeight() / 2, 180, 50);
    }
    camera_->Draw();
    renderer_->Draw(camera_);

    //UI
    DrawSpriteLeftTop(timer_tex_, Vector2(Graphical::GetWidth() / 2, 50), 0.0f, Vector2(250, 100),
                      Color(1.0f, 1.0f, 1.0f, 1.0f));
    int itimeer = static_cast<int>(timer_);
    std::wstring time = std::to_wstring(itimeer);
    Text::WriteText(time.c_str(), text_format_, brush_, Graphical::GetWidth() / 2 - 45, 25, 180, 50);
}

void Game::UpdateResult()
{
    static bool changed = false;
    if (GetChangeScene() == 1 && !changed)
    {
        result_tex_ = LoadTexture("data/texture/UI/clear.png");
    }
    else if (GetChangeScene() == 2 && !changed)
    {
        result_tex_ = LoadTexture("data/texture/UI/over.png");
    }
    if (!changed)
    {
        changed = true;
        button_title_ = LoadTexture("data/texture/UI/button_title.png");
        button_restart_ = LoadTexture("data/texture/UI/button_restart.png");
    }

    //選択画面描画

    if (result_)
    {
        //ゲームに戻る
        if (Input::GetKeyDown(0, Input::East))
        {
            result_ = false;
        }
        //ステージ選択画面に戻る
        if (Input::GetKeyDown(0, Input::South))
        {
            scene_mngr_->ChangeScene(SCENE_MENU);
        }

        return;
    }

    //一回のみ
    result_ = true;
}

void Game::DrawResult()
{
    camera_->Draw();
    renderer_->Draw(camera_);
    DrawSprite(result_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2 + 200), 0.0f, Vector2(327, 100),
               Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(button_title_, Vector2(Graphical::GetWidth() / 2 - 200, Graphical::GetHeight() / 2 - 200), 0.0f,
               Vector2(120, 68), Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(button_restart_, Vector2(Graphical::GetWidth() / 2 + 200, Graphical::GetHeight() / 2 - 200), 0.0f,
               Vector2(120, 68), Color(1.0f, 1.0f, 1.0f, 1.0f));
}
