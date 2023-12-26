#include "game.h"
#include "dark.h"
#include "fire.h"
//#include "mapmngr.h"
#include "thunder.h"
#include "wind.h"
//#include "renderer.h"

Game::Game(SceneMngr* scene_mngr)
    : GameBase(scene_mngr)
{
    mapmngr_ = new MapMngr("data/map/stage1_test.csv", this);
    int playertex = LoadTexture("data/texture/player.png");
    Player *player_ = new Player(mapmngr_->GetPlayerSpawn(), 0.0f, playertex, Vector2(0.0f, 0.0f), mapmngr_);
    players_.push_back(player_);
    camera_ = new Camera(player_);
}


void Game::Update()
{
    coll_mngr_->Update();
    mapmngr_->Update();
    GetPlayer()->Update();
    camera_->Update();
    renderer_->Update();
    if (GetPlayer()->GetChangeSceneFlag())
    {
        scene_mngr_->ChangeScene(SCENE_RESULT);
    }
}

void Game::Draw()
{
    camera_->Draw();
    renderer_->Draw(camera_ ->GetCameraOffset());
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
    player_->DebugMenu();
}

