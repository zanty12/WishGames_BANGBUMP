#pragma once
#include <map>
#include "multi_character_select_frame.h"
#include "multiplay.h"
#include "multi_mode.h"
#include "multi_runenum.h"
#include "lib/math.h"
#include "asset.h"
#include "prep.h"
#include "follow.h"
#include "video.h"
#include "asset.h"

class MultiPlayServer;
class MultiPlayClient;
/***********************************************************
	Server
************************************************************/
class MultiPlayCharacterSelectModeServerSide : public MultiPlayModeServerSide {
private:
	MultiPlayServer *game_ = nullptr;
	std::map<int, int> status;
	float moveNextSceneTime = 0.0f;

private:
	/// <summary>
	/// 属性入力選択
	/// </summary>
	void AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType);

	void PlayerUpdate(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);

	/// <summary>
	/// 属性作成
	/// </summary>
	ServerAttribute *CreateAttribute(ATTRIBUTE_TYPE type, ServerPlayer * player);

public:
	MultiPlayCharacterSelectModeServerSide(MultiPlayServer *game)
		: MultiPlayModeServerSide(L"CharacterSelect"), game_(game) {
	}

	void Release(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override;
	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override;
	void UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override;
	void CreateResponse(Storage &out) override;

	bool IsPlayerMove(void) const override { return false; }
	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};






/***********************************************************
	Client
************************************************************/
class MultiPlayCharacterSelectModeClientSide : public MultiPlayModeClientSide {
private:

	/// <summary>
	/// 属性作成
	/// </summary>
	ClientAttribute *CreateAttribute(ATTRIBUTE_TYPE type, ClientPlayer *player);

private:
	RESPONSE_CHARACTER_SELECT res;
	std::map<int, CharacterSelectFrameClientSide> characters;
	MultiPlayClient *game_ = nullptr;
	Video *moveAttributeVideo[4]= {};
	Video *attackAttributeVideo[4]= {};
	int bootTexNo[ATTRIBUTE_TYPE_NUM] = {};
	int handTexNo[ATTRIBUTE_TYPE_NUM] = {};
	int playerTexNo[4] = {};									// キャラ画像
	int charFramePTexNo[4] = {};								// キャラフレーム
	int charFrameTexNo = -1;									// キャラのフレーム
	int charSelectArrow = -1;									// 矢印
	int charReadyTexNo = -1;									// レディーソード


	int return_tex_ = -1;										// テクスチャ（戻る）
	int confirm_tex_ = -1;										// テクスチャ（進む）
	int a_tex_ = -1;											// テクスチャ（Aボタン）
	int b_tex_ = -1;											// テクスチャ（Bボタン）
	int select_tex_ = -1;										// テクスチャ（選択）
	int stick_tex_ = -1;										// テクスチャ（スティック）
	int match_tex_ = -1;										// テクスチャ（マッチメイキング）

	int character_select_so = -1;								// キャラクター選択時の音

private:
	void CharacterDraw(int id, int maxIdx, bool isShow, float width, float height, float gap, int moveAttribute, int attackAttribute, float moveAttributeSmooth, float attackAttributeSmooth);

public:
	MultiPlayCharacterSelectModeClientSide(MultiPlayClient * game) : MultiPlayModeClientSide(L"CharacterSelect"), game_(game) {
		map_->backBGTexNo = LoadTexture("data/texture/BG/UI_elemental_BG.png");

		// プレイヤーテクスチャの取得
		bootTexNo[0] = LoadTexture("data/texture/UI/player_boot1.png");
		bootTexNo[1] = LoadTexture("data/texture/UI/player_boot2.png");
		bootTexNo[2] = LoadTexture("data/texture/UI/player_boot3.png");
		bootTexNo[3] = LoadTexture("data/texture/UI/player_boot4.png");
		handTexNo[0] = LoadTexture("data/texture/UI/player_hand1.png");
		handTexNo[1] = LoadTexture("data/texture/UI/player_hand2.png");
		handTexNo[2] = LoadTexture("data/texture/UI/player_hand3.png");
		handTexNo[3] = LoadTexture("data/texture/UI/player_hand4.png");
		playerTexNo[0] = LoadTexture("data/texture/UI/player1_base.png");
		playerTexNo[1] = LoadTexture("data/texture/UI/player2_base.png");
		playerTexNo[2] = LoadTexture("data/texture/UI/player3_base.png");
		playerTexNo[3] = LoadTexture("data/texture/UI/player4_base.png");

		// UIテクスチャの取得
		a_tex_ = LoadTexture("data/texture/UI/A.png");
		b_tex_ = LoadTexture("data/texture/UI/B.png");
		return_tex_ = LoadTexture("data/texture/UI/back.png");
		confirm_tex_ = LoadTexture("data/texture/UI/go.png");
		select_tex_ = LoadTexture("data/texture/UI/select.png");
		stick_tex_ = LoadTexture("data/texture/UI/stick.png");
		match_tex_ = LoadTexture("data/texture/UI/menu/UI_matchmaking.png");

		// キャラ選択
		charFrameTexNo = LoadTexture("data/texture/UI/UI_frame_base.png");
		charSelectArrow = LoadTexture("data/texture/UI/UI_elemental_select.png");
		charReadyTexNo = LoadTexture("data/texture/UI/UI_ready.png");
		charFramePTexNo[0] = LoadTexture("data/texture/UI/UI_frame_player1.png");
		charFramePTexNo[1] = LoadTexture("data/texture/UI/UI_frame_player2.png");
		charFramePTexNo[2] = LoadTexture("data/texture/UI/UI_frame_player3.png");
		charFramePTexNo[3] = LoadTexture("data/texture/UI/UI_frame_player4.png");

		soNo = LoadSound("data/sound/BGM/select_element_BGM.wav");
		character_select_so = LoadSound("data/sound/SE/select_finish.wav");

		map_->backBGColor = Color::White;
		map_->middleBGColor = Color::White;
		map_->frontBGColor = Color::White;


		// ブロック非表示
		isBlockShow = false;
		
		// ビデオの描画設定
		Vector2 screenScale = Vector2(static_cast<float>(Graphical::GetWidth()), static_cast<float>(Graphical::GetHeight()));
		Vector2 screenCenter = screenScale * 0.5f;
		Vector2 videoScale = Vector2(853.3 * screenScale.x / 1920, 480 * screenScale.y / 1080);
		Vector2 moveVideoPos = Vector2(screenCenter.x + videoScale.x * 0.5f, screenCenter.y);
		Vector2 attackVideoPos = Vector2(screenCenter.x - videoScale.x * 0.5f, screenCenter.y);

		// 移動ビデオ
		for (int i = 0; i < ATTRIBUTE_TYPE_NUM; i++) {
			// ビデオのロード
			Video *video = nullptr;
			switch (i) {
			case ATTRIBUTE_TYPE_FIRE: video = new Video("./data/video/fire_move.mp4"); break;
			case ATTRIBUTE_TYPE_DARK: video = new Video("./data/video/dark_move.mp4"); break;
			case ATTRIBUTE_TYPE_THUNDER: video = new Video("./data/video/thunder_move.mp4"); break;
			case ATTRIBUTE_TYPE_WIND: video = new Video("./data/video/wind_move.mp4"); break;
			}

			// ビデオの設定
			video->SetSize(videoScale);
			video->SetLoop(true);
			video->SetWindowPos(moveVideoPos);

			moveAttributeVideo[i] = video;
		}
		// 攻撃ビデオ
		for (int i = 0; i < ATTRIBUTE_TYPE_NUM; i++) {
			// ビデオのロード
			Video *video = nullptr;
			switch (i) {
			case ATTRIBUTE_TYPE_FIRE: video = new Video("./data/video/fire_attack.mp4"); break;
			case ATTRIBUTE_TYPE_DARK: video = new Video("./data/video/dark_attack.mp4"); break;
			case ATTRIBUTE_TYPE_THUNDER: video = new Video("./data/video/thunder_attack.mp4"); break;
			case ATTRIBUTE_TYPE_WIND: video = new Video("./data/video/wind_attack.mp4"); break;
			}

			// ビデオの設定
			video->SetSize(videoScale);
			video->SetLoop(true);
			video->SetWindowPos(attackVideoPos);

			attackAttributeVideo[i] = video;
		}
	}

	~MultiPlayCharacterSelectModeClientSide() {
		for (int i = 0; i < ATTRIBUTE_TYPE_NUM; i++) {
			if (moveAttributeVideo[i]) {
				delete moveAttributeVideo[i];
			}
		}
		for (int i = 0; i < ATTRIBUTE_TYPE_NUM; i++) {
			if (attackAttributeVideo[i]) {
				delete attackAttributeVideo[i];
			}
		}
	}

	void DrawStart(RESPONSE_PLAYER &players, Vector2 offset) override;
	void DrawResult(RESPONSE_PLAYER &players, Vector2 offset) override;
	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override;
	void DrawUI(RESPONSE_PLAYER &players) override;
	void ParseResponse(Storage &in) override;
	void Release(RESPONSE_PLAYER &players) override;

	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};
