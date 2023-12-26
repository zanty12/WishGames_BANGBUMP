#pragma once
#include "multi_mode.h"
#include "multi_client_gameobject.h"
#include "sprite.h"
#include "time.h"


class MultiPlayAreaCaptureModeServerSide : public MultiPlayServerSide {
public:
	struct Area : public RESPONSE_AREA_CAPTURE::DESC {
		float captureMaxTime = 5.0f;				// ��̂̍ő厞��
		float captureNowTime = 0.0f;				// ��̎���
	};

private:
	std::list<Area> areas;
	std::list<Area> activeAreas;





private:
	// ��̂��A�N�e�B�u�ɂ���
	void ActiveArea(void);
	// ��̂̃A�N�e�B�u�X�V
	void ActiveUpdate(void);
	// ��̂̍X�V
	void CaptureUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients);
	// ��̍폜
	void DestroyUpdate(void);
	// �X�|�[��
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
