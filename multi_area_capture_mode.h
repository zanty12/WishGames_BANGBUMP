#pragma once
#include "multi_mode.h"
#include "sprite.h"
#include "time.h"
#include "asset.h"

/***********************************************************
	Server
************************************************************/
class MultiPlayAreaCaptureModeServerSide : public MultiPlayModeServerSide {
public:
	struct Area : public RESPONSE_AREA_CAPTURE::DESC {
		float captureMaxTime = 5.0f;				// 占領の最大時間
		float captureNowTime = 0.0f;				// 占領時間
		Area(Vector2 position) { this->position = position; }
	};

private:
	std::list<int> inOrder;
	std::list<Area> areas;
	std::list<Area> activeAreas;
	float radius = 5.0f;							// 半径
	float areaGetTime = 5.0f;						// 占領の最大時間
	DWORD preTime = 0ul;
	DWORD curTime = 0ul;





private:
	// 占領をアクティブにする
	void ActiveArea(void);
	// 占領のアクティブ更新
	void ActiveUpdate(void);
	// 占領の更新
	void CaptureUpdate(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);
	// 占領削除
	void DestroyUpdate(void);

public:
	MultiPlayAreaCaptureModeServerSide();

	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override;

	void CreateResponse(Storage &out) override;

	MULTI_MODE GetMode(void) const override { return AREA_CAPTURE; }
};






/***********************************************************
	Client
************************************************************/
class MultiPlayAreaCaptureModeClientSide : public MultiPlayModeClientSide {
private:
	RESPONSE_AREA_CAPTURE res;
	int areaTexNo = -1;
	GameObjectClientSide playerObject;
	MultiAnimator anim;


public:
	MultiPlayAreaCaptureModeClientSide() : 
		areaTexNo(LoadTexture("data/texture/area_capture.png")),
		MultiPlayModeClientSide(L"AreaCapture") {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage1_back));
		map_->middleBGTexNo = LoadTexture("data/texture/BG/BG_stage1_middle.png");
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage1_front));
		anim = MultiAnimator(areaTexNo, 5, 6, 0, 29, true);
		soNo = LoadSound("data/sound/BGM/stage1_BGM.wav");
		descTexNo = LoadTexture("data/texture/description/areacapture_description.png");

		stageNameTexNo = LoadTexture("data/texture/UI/title/UI_stage1.png");
	}

	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override;

	void ParseResponse(Storage &in);

	MULTI_MODE GetMode(void) const override { return AREA_CAPTURE; }
};
