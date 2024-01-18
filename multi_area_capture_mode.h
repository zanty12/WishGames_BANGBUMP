//#pragma once
//#include "multi_mode.h"
//#include "multi_client_gameobject.h"
//#include "sprite.h"
//#include "time.h"
//
///***********************************************************
//	Server
//************************************************************/
//class MultiPlayAreaCaptureModeServerSide : public MultiPlayServerSide {
//public:
//	struct Area : public RESPONSE_AREA_CAPTURE::DESC {
//		float captureMaxTime = 5.0f;				// ��̂̍ő厞��
//		float captureNowTime = 0.0f;				// ��̎���
//	};
//
//private:
//	std::list<Area> areas;
//	std::list<Area> activeAreas;
//
//
//
//
//
//private:
//	// ��̂��A�N�e�B�u�ɂ���
//	void ActiveArea(void);
//	// ��̂̃A�N�e�B�u�X�V
//	void ActiveUpdate(void);
//	// ��̂̍X�V
//	void CaptureUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients);
//	// ��̍폜
//	void DestroyUpdate(void);
//	// �X�|�[��
//	void Spawn(Vector2 position);
//
//public:
//	MultiPlayAreaCaptureModeServerSide(GameBase *game);
//
//	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) override;
//
//	void CreateResponse(Storage &out) override;
//
//	MULTI_MODE GetMode(void) const override { return AREA_CAPTURE; }
//};
//
//
//
//
//
//
///***********************************************************
//	Client
//************************************************************/
//class MultiPlayAreaCaptureModeClientSide : public MultiPlayClientSide {
//private:
//	RESPONSE_AREA_CAPTURE res;
//	int areaTexNo = -1;
//	ClientGameObject playerObject;
//	Animator *anim = nullptr;
//
//
//public:
//	MultiPlayAreaCaptureModeClientSide(GameBase *game) : 
//		areaTexNo(LoadTexture("data/texture/area_capture.png")), 
//		anim(new Animator(&playerObject)),
//		MultiPlayClientSide(new MapMngr("data/map/MultiPlay_Map1.csv", game)) {
//
//	}
//	~MultiPlayAreaCaptureModeClientSide() {
//		anim->Discard();
//	}
//
//	void Draw(RESPONSE_PLAYER &players) override;
//
//	void ParseResponse(Storage &in);
//
//	MULTI_MODE GetMode(void) const override { return AREA_CAPTURE; }
//};
