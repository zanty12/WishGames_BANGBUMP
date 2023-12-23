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





private:
	// ��̂̍X�V
	void CaptureUpdate(std::list<CLIENT_DATA_SERVER_SIDE>& clients) {
		using namespace PHYSICS;

		// �G���A
		for (auto& area : areas) {
			Vertex1 areaCollider = Vertex1(area.position, area.radius);	// �G���A�̃R���C�_�[
			int touchID = -1;													// �G���A�ɐG��Ă���ID

			// �v���C���[
			for (auto& client : clients) {
				Vertex1 clientCollider = Vertex1(client.player_->GetPos(), 1);	// �v���C���[�̃R���C�_�[

				// �Փ˔���
				if (Collider2D::Touch(areaCollider, clientCollider)) {
					int id = client.header.id;									// �v���C���[��ID

					if (area.id == id ||	// �����̃G���A�Ȃ�
						area.id == -1 ||	// ����̃G���A�ł��Ȃ��Ȃ�
						touchID == -1) {	// touchID�������l�Ȃ�
						touchID = id;			// �X�V����
					}

					// �N����̂��Ă��Ȃ��Ȃ��̏�Ԃɂ���
					if (area.id == -1)
						area.id = client.header.id;

				}
			}

			// ������G��Ă��Ȃ��Ȃ�
			if (touchID == -1) continue;

			// ��̂������Ă���Ȃ�Q�[�W�̍X�V
			if (area.id == touchID) {
				area.captureNowTime += Time::GetDeltaTime();
			}
			// �ʂ̃v���C���[�ɐ�̂��ꂽ�Ȃ珉����
			else {
				area.id = touchID;
				area.captureNowTime = 0.0f;
			}

			area.captureRatio = area.captureNowTime / area.captureMaxTime;		// �����v�Z
		}
	}
	// ��̍폜
	void DestoryUpdate() {
		for (auto iterator = areas.begin(); iterator == areas.end(); iterator++) {
			if (1.0f < iterator->captureRatio) {
				auto iterator__ = iterator;
				iterator__--;
				areas.erase(iterator);
				iterator = iterator__;
			}
		}
	}
	// �X�|�[��
	void Spawn(Vector2 position) {
		// �G���A�̐���
		Area area;
		area.position = position;
		areas.push_back(area);
	}
public:
	MultiPlayAreaCaptureModeServerSide(MapMngr *map_) : MultiPlayServerSide(map_) {
		Spawn(Vector2(900, 600) / 2);
	}

	void Update(std::list<CLIENT_DATA_SERVER_SIDE>& clients) override {
		CaptureUpdate(clients);
		DestoryUpdate();
	}

	void CreateResponse(Storage &out) override {
		RESPONSE_AREA_CAPTURE res;
		// �G���A�̐ݒ�
		for (auto& area : areas)
			res.areas.push_back(area);

		// ���X�|���X�쐬
		res.CreateResponse(out);
	}
};






class MultiPlayAreaCaptureModeClientSide : public MultiPlayClientSide {
private:
	RESPONSE_AREA_CAPTURE res;
	int areaTexNo = -1;
	ClientGameObject playerObject;
	Animator anim;
	//MapMngr map_ = MapMngr("data/map/1.csv", nullptr);


public:
	MultiPlayAreaCaptureModeClientSide(MapMngr *map) : areaTexNo(LoadTexture("data/texture/area_capture.png")), anim(Animator(&playerObject, 1, true, 1,1,1)), MultiPlayClientSide(map){ }

	void Draw(void) override {
		// �G���A�̕`��
		for (auto area : res.areas) {
			anim.SetTexNo(areaTexNo);
			anim.SetPos(area.position);
			anim.SetScale(Vector2(area.radius, area.radius));
			anim.Draw();
		}
	}

	void ParseResponse(Storage& in) override {
		res = RESPONSE_AREA_CAPTURE();
		// ���X�|���X���
		res.ParseResponse(in);
	}
};
