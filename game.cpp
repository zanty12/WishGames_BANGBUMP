#include "game.h"
#include "dark.h"
#include "fire.h"

#include "thunder.h"
#include "wind.h"
#include <thread>

#include "asset.h"
#include "text.h"

Game::Game(SceneMngr* scene_mngr)
    : GameBase(scene_mngr)
{

   //mapmngr_ = new MapMngr(Asset::GetAsset(single_stage_1).c_str(), this);
   mapmngr_ = new MapMngr("data/map/2.csv", this);
   //int playertex = LoadTexture("data/texture/player.png");
    mapmngr_ = new MapMngr("data/map/2.csv", this);
    Player* player_ = new Player(mapmngr_->GetPlayerSpawn(), 0.0f, Vector2(0.0f, 0.0f), mapmngr_);
    //mapmngr_ = new MapMngr(Asset::GetAsset(single_stage_1).c_str(), this);
    text_format_ = Text::MakeTextFormat(L"ワープロ明朝", 50, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL);
    brush_ = Text::MakeBrush(Color(1.0f, 1.0f, 1.0f, 1.0f));
    timer_tex_ = LoadTexture(Asset::GetAsset(timer));
    //int playertex = LoadTexture("data/texture/player.png");

    /*Player *player_ = new Player(mapmngr_->GetPlayerSpawn(), 0.0f, Vector2(0.0f, 0.0f), mapmngr_);
    players_.push_back(player_);
    camera_ = new Camera(player_);*/
}


void Game::Update()
{
    coll_mngr_->Update();
    std::thread map(&MapMngr::Update, mapmngr_);
    //mapmngr_->Update();
    std::thread enemy(&EnemyMngr::Update, mapmngr_->GetEnemyMngr());
    std::thread player(&Player::Update, GetPlayer());
    //GetPlayer()->Update();
    //std::thread projectile(&ProjectileMngr::Update, projectile_mngr_);
    projectile_mngr_->Update();
    //std::thread orb(&SkillOrbMngr::Update, orb_mngr_);
    //orb_mngr_->Update();
    //projectile.join();
    //orb.join();
    std::thread camera(&Camera::Update, camera_);
    //camera_->Update();
    std::thread renderer(&Renderer::Update, renderer_);
    //renderer_->Update();
    map.join();
    enemy.join();
    player.join();
    camera.join();
    renderer.join();
    if (GetPlayer()->GetChangeSceneFlag())
    {
        scene_mngr_->ChangeScene(SCENE_RESULT);
    }
    timer_ -= Time::GetDeltaTime();
}

void Game::Draw()
{
    camera_->Draw();
    renderer_->Draw(camera_ ->GetCameraOffset());

    //UI
    int itimeer = static_cast<int>(timer_);
    std::wstring time = L"残り時間: ";
    time += std::to_wstring(itimeer);
    Text::WriteText(time.c_str(),Graphical::GetWidth()/ 2, 100, 100, 50);
}

Player *Game::GetPlayer() {
    return *GetPlayers().begin();
}

void Game::DebugMenu()
{
    auto player_ = GetPlayer();

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
    if(player_->GetAttribute() == nullptr)
        preview = u8"無属性";
    if(player_->GetAttackAttribute() == nullptr)
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
    if(ImGui::Button("map1"))
    {
        delete mapmngr_;
        coll_mngr_->CheckDiscard();
        renderer_->CheckDiscard();
        mapmngr_ = new MapMngr("data/map/stage1_test.csv", this);
    }
    ImGui::End();
    player_->DebugMenu();
}

