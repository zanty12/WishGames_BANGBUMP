#include "multi_area_capture_mode.h"

/***********************************************************
	Server
************************************************************/
void MultiPlayAreaCaptureModeServerSide::ActiveArea(void) {
	if (areas.size() == 0) return;		// �A�N�e�B�u�ɂ���G���A���Ȃ��Ȃ�I��

	auto iterator = areas.begin();
	activeAreas.push_back(*iterator);	// �G���A���A�N�e�B�u�ɂ���
	areas.erase(iterator);				// �G���A����͍폜����
}

void MultiPlayAreaCaptureModeServerSide::ActiveUpdate(void) {
	if (activeAreas.size()) return;		// �A�N�e�B�u�ȃG���A���܂�����Ȃ�I��

	ActiveArea();
}

void MultiPlayAreaCaptureModeServerSide::CaptureUpdate(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	using namespace PHYSICS;

	// �G���A
	for (auto &area : activeAreas) {
		Vertex1 areaCollider = Vertex1(area.position, area.radius);			// �G���A�̃R���C�_�[
		CLIENT_DATA_SERVER_SIDE *pTouchClient = nullptr;					// �G���A�ɐG��Ă���v���C���[

		// �v���C���[
		for (auto &kvp : clients) {
			auto &client = kvp.second;
			auto &player = client.player_;
			Vertex1 clientCollider = Vertex1(player->transform.position, player->radius);	// �v���C���[�̃R���C�_�[

			// �Փ˔���
			if (Collider2D::Touch(areaCollider, clientCollider)) {
				int id = client.header.id;									// �v���C���[��ID

				if (area.id == id ||	// �����̃G���A�Ȃ�
					area.id == -1 ||	// ����̃G���A�ł��Ȃ��Ȃ�
					pTouchClient == nullptr) {	// �N�����Ȃ��Ȃ�
					pTouchClient = &client;		// �X�V����
					area.id = id;
				}
			}
		}

		// ������G��Ă��Ȃ��Ȃ�
		if (pTouchClient == nullptr) continue;

		// ��̂������Ă���Ȃ�Q�[�W�̍X�V
		if (area.id == pTouchClient->header.id) {
			area.captureNowTime += Time::GetDeltaTime();
		}
		// �ʂ̃v���C���[�ɐ�̂��ꂽ�Ȃ珉����
		else {
			area.id = pTouchClient->header.id;
		}

		area.captureRatio = area.captureNowTime / area.captureMaxTime;		// �����v�Z

		// ��̊��������Ȃ瓾�_
		if (1.0f <= area.captureRatio) pTouchClient->player_->score++;
	}
}
// ��̍폜
void MultiPlayAreaCaptureModeServerSide::DestroyUpdate(void) {
	auto iterator = activeAreas.begin();

	while (iterator != activeAreas.end()) {
		// ��̊���
		if (1.0f < iterator->captureRatio) {
			// �폜
			auto tmp = iterator++;
			activeAreas.erase(tmp);
		}
		else iterator++;
	}
}

MultiPlayAreaCaptureModeServerSide::MultiPlayAreaCaptureModeServerSide()
	: MultiPlayModeServerSide(new MultiMap("data/map/MultiPlay_Map1.csv")) {
	// ��̃f�[�^���擾
	for (auto &areaPosition : GetMap()->GetAreaCaptures()) {
		areas.push_back(Area(areaPosition));
	}

	maxTime_ = 1000.0f;
}

// �Q�[���A�b�v�f�[�g
void MultiPlayAreaCaptureModeServerSide::Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	ActiveUpdate();
	CaptureUpdate(clients);
	DestroyUpdate();
}

void MultiPlayAreaCaptureModeServerSide::CreateResponse(Storage &out) {
	RESPONSE_AREA_CAPTURE res;
	// �G���A�̐ݒ�
	for (auto &area : activeAreas) {
		res.areas.push_back(area);
	}

	// ���X�|���X�쐬
	res.CreateResponse(out);
}







/***********************************************************
	Client
************************************************************/
void MultiPlayAreaCaptureModeClientSide::Draw(RESPONSE_PLAYER &players, Vector2 offset) {
	// �G���A�̕`��
	for (auto &area : res.areas) {
		Vector2 pos = area.position - offset;
		float rot = 0.0f;
		Vector2 scl = Vector2(area.radius, area.radius);
		Color col = Color::White;
		anim.Draw(pos, rot, scl, col);
		return;
	}
}

void MultiPlayAreaCaptureModeClientSide::ParseResponse(Storage &in) {
	res = RESPONSE_AREA_CAPTURE();
	// ���X�|���X���
	res.ParseResponse(in);
}
