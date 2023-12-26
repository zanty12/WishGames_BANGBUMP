#pragma once
#include "multi_mode.h"
#include "multi_client_gameobject.h"
#include "sprite.h"
#include "time.h"


class MultiPlayAreaCaptureModeServerSide : public MultiPlayServerSide {
public:
	struct Area : public RESPONSE_AREA_CAPTURE::DESC {
		float captureMaxTime = 5.0f;				// 占領の最大時間
		float captureNowTime = 0.0f;				// 占領時間
	};

private:
	std::list<Area> areas;
	std::list<Area> activeAreas;





private:
	// 占領をアクティブにする
	void ActiveArea(void);
	// 占領のアクティブ更新
	void ActiveUpdate(void);
	// 占領の更新
	void CaptureUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients);
	// 占領削除
	void DestroyUpdate(void);
	// スポーン
	void Spawn(Vector2 position);

protected:
	void GameUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients) override;

public:
	MultiPlayAreaCaptureModeServerSide(MapMngr *map_);

	void CreateResponse(Storage &out) override;
};






class MultiPlayAreaCaptureModeClientSide : public MultiPlayClientSide {
private:
	RESPONSE_AREA_CAPTURE res;
	int areaTexNo = -1;
	ClientGameObject playerObject;
	Animator anim;


public:
	MultiPlayAreaCaptureModeClientSide(MapMngr *map) : areaTexNo(LoadTexture("data/texture/area_capture.png")), anim(Animator(&playerObject, 1, true, 1,1,1)), MultiPlayClientSide(map){ }

	void Draw(void) override;

	void ParseResponse(Storage &in);
};
