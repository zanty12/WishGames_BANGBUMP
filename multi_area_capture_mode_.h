#pragma once
#include "multi_mode.h"
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
			Vertex4 areaCollider = Vertex4(area.position, 0.0f, area.scale);	// �G���A�̃R���C�_�[
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

			// ��̂������Ă���Ȃ�Q�[�W�̍X�V
			if (area.id == touchID) {
				area.captureNowTime = Time::GetDeltaTime();
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
	MultiPlayAreaCaptureModeServerSide(MapMngr* map) : MultiPlayServerSide(map) { }

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

public:
	void Draw(CLIENT_DATA_CLIENT_SIDE client) override {
		

	}

	void ParseResponse(Storage& in) override {
		// ���X�|���X���
		res.ParseResponse(in);
	}
};