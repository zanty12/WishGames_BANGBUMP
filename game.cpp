#include "game.h"

#include "dark.h"
#include "fire.h"
#include "mapmngr.h"
#include "thunder.h"
#include "wind.h"

Game::Game() {
    mapmngr_ = new MapMngr("data/map/1.csv",this);
    player_ = new Player(mapmngr_->GetPlayerSpawn(), 0.0f, LoadTexture("player.jpg"), Vector2(0.0f, 0.0f), mapmngr_);
}

void Game::Update() {
    mapmngr_->Update();
    player_->Update();
}
void Game::Draw(){
    mapmngr_->Draw();
    player_->Draw();
}

void Game::DebugMenu()
{
    ImGui::Begin("Game");
    ImGui::Text(u8"プレイヤー座標");
    ImGui::Text("x:%f, y: %f", player_->GetPos().x, player_->GetPos().y);
    static std::string preview = u8"無属性";
    if(ImGui::BeginCombo(u8"プレイヤー移動属性", preview.c_str()))
    {
        if (ImGui::Selectable("Fire", false)) {
            preview = "Fire";
            player_->SetAttribute(new Fire(player_));
        }
        if (ImGui::Selectable("Wind", false)) {
            preview = "Wind";
            player_->SetAttribute(new Wind(player_));
        }
        if (ImGui::Selectable("Thunder", false)) {
            preview = "Thunder";
            player_->SetAttribute(new Thunder(player_));
        }
        if (ImGui::Selectable("Dark", false)) {
            preview = "Dark";
            player_->SetAttribute(new Dark(player_));
        }
        ImGui::EndCombo();
    }
    ImGui::End();
}
