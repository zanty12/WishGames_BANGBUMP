#include "game.h"
#include "mapmngr.h"

Game::Game() {
    mapmngr_ = new MapMngr("data/map/1.csv",this);
    player_ = new Player(mapmngr_->GetPlayerSpawn(), 0.0f, LoadTexture("player.jpg"), Vector2(0.0f, 0.0f));
}

void Game::Update() {
    mapmngr_->Update();
    //player_->Update();
}
void Game::Draw(){
    mapmngr_->Draw();
    player_->Draw();
}