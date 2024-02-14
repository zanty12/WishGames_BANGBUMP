#pragma once
#include <list>
#include "multiplay.h"
#include "multi_mode.h"
#include "time.h"
#include "asset.h"
#include "video.h"
#include "move_scene_anim.h"


/*******************************************************
  Server
********************************************************/
class MultiPlayLastResultModeServerSide : public MultiPlayModeServerSide {
public:
	MultiPlayLastResultModeServerSide() : MultiPlayModeServerSide(L"LastResult") { }

	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override { };
	void UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override { };
	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return LAST_RESULT; }

};





/*******************************************************
  Client
********************************************************/
class MultiPlayLastResultModeClientSide : public MultiPlayModeClientSide {
public:
	std::list<CLIENT_DATA_CLIENT_SIDE> ranking;

	int iconTexNo[4] = {};
	int winTexNo[4] = {};
	int barTexNo[4] = {};
	int nameTexNo[4] = {};
	int victoryTexNo = -1;
	int defeatTexNo = -1;
	int frameTexNo = -1;
	Video *video = nullptr;
	MultiPlayClient *game = nullptr;

	MultiPlayLastResultModeClientSide(MultiPlayClient *game) : game(game), MultiPlayModeClientSide(L"LastResult") {
		MoveScene::Move(Color(0, 0, 0, 0));
		soNo = LoadSound("data/sound/BGM/stage3_BGM.wav");
		iconTexNo[0] = LoadTexture("data/texture/UI/result/UI_player1.png");
		iconTexNo[1] = LoadTexture("data/texture/UI/result/UI_player2.png");
		iconTexNo[2] = LoadTexture("data/texture/UI/result/UI_player3.png");
		iconTexNo[3] = LoadTexture("data/texture/UI/result/UI_player4.png");
		winTexNo[0] = LoadTexture("data/texture/UI/result/UI_result_player1.png");
		winTexNo[1] = LoadTexture("data/texture/UI/result/UI_result_player2.png");
		winTexNo[2] = LoadTexture("data/texture/UI/result/UI_result_player3.png");
		winTexNo[3] = LoadTexture("data/texture/UI/result/UI_result_player4.png");
		barTexNo[0] = LoadTexture("data/texture/UI/result/UI_result_1.png");
		barTexNo[1] = LoadTexture("data/texture/UI/result/UI_result_2.png");
		barTexNo[2] = LoadTexture("data/texture/UI/result/UI_result_3.png");
		barTexNo[3] = LoadTexture("data/texture/UI/result/UI_result_4.png");
		nameTexNo[0] = LoadTexture("data/texture/UI/result/UI_result_text1.png");
		nameTexNo[1] = LoadTexture("data/texture/UI/result/UI_result_text2.png");
		nameTexNo[2] = LoadTexture("data/texture/UI/result/UI_result_text3.png");
		nameTexNo[3] = LoadTexture("data/texture/UI/result/UI_result_text4.png");
		victoryTexNo = LoadTexture("data/texture/UI/result/VICTORY.png");
		defeatTexNo = LoadTexture("data/texture/UI/result/DEFEAT.png");
		frameTexNo = LoadTexture("data/texture/UI/result/UI_result_frame.png");

		isBlockShow = false;


		video = new Video("./data/video/UI_result_BG.mp4");
		Vector2 screen = Vector2(Graphical::GetWidth(), Graphical::GetHeight());
		video->SetSize(screen);
		video->SetWindowPos(screen * 0.5f);
		video->SetLoop(true);
	}
	~MultiPlayLastResultModeClientSide() {
		delete video;
	}


	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override;
	void DrawStart(RESPONSE_PLAYER &players, Vector2 offset) override;

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return LAST_RESULT; }
};