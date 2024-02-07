#include "multi_area_capture_mode.h"
#include "ini.h"

/***********************************************************
	Server
************************************************************/
void MultiPlayAreaCaptureModeServerSide::ActiveArea(void) {
	if (areas.size() == 0) return;		// �A�N�e�B�u�ɂ���G���A���Ȃ��Ȃ�I��

	auto iterator = areas.begin();

	// �X�V����
	iterator->captureMaxTime = areaGetTime;
	iterator->radius = radius;

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
		int inPlayerNum = 0;												// �̈�ɓ����Ă���v���C���[�̐�


		// �v���C���[
		for (auto &kvp : clients) {
			auto &client = kvp.second;
			auto &player = client.player_;
			Vertex1 clientCollider = Vertex1(player->transform.position, 0.0f);	// �v���C���[�̃R���C�_�[

			// �Փ˔���
			if (Collider2D::Touch(areaCollider, clientCollider)) {
				int id = client.header.id;									// �v���C���[��ID
				inPlayerNum++;

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
			area.captureNowTime += (curTime - preTime) * 0.001f;
		}
		// �ʂ̃v���C���[�ɐ�̂��ꂽ�Ȃ珉����
		else {
			area.id = pTouchClient->header.id;
		}

		area.captureRatio = area.captureNowTime / area.captureMaxTime;		// �����v�Z

		// ��̊��������Ȃ瓾�_
		if (1.0f <= area.captureRatio) {
			pTouchClient->player_->score++;
			// �X�L���I�[�u���h���b�v������
			map_->DropSkillOrb(15, area.position, 10.0f);
		}
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
	: MultiPlayModeServerSide(new MultiMap(MAP_PATH + "MultiPlay_Map1.csv", MULTIPLAY_RUN_TYPE_SERVER), L"AreaCapture") {
	// ��̃f�[�^���擾
	for (auto &areaPosition : GetMap()->GetAreaCaptures()) {
		areas.push_back(Area(areaPosition));
	}

	radius = ini::GetFloat(PARAM_PATH + L"mode.ini", L"AreaCapture", L"radius");
	areaGetTime = ini::GetFloat(PARAM_PATH + L"mode.ini", L"AreaCapture", L"areaGetTime");
	curTime = preTime = timeGetTime();
}

// �Q�[���A�b�v�f�[�g
void MultiPlayAreaCaptureModeServerSide::Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	curTime = timeGetTime();
	ActiveUpdate();
	CaptureUpdate(clients);
	DestroyUpdate();
	preTime = curTime;
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
		Vector2 scl = Vector2(area.radius, area.radius) * 2.0f;
		Color col = Color::White * 0.5f;
		anim.Draw(pos, rot, scl, col);
		DrawSpriteCircleEffect(anim.texNo, pos, rot, scl, col, Vector2::Zero, Vector2::One, area.captureRatio);
		anim.SetActive(false);
	}
}

void MultiPlayAreaCaptureModeClientSide::ParseResponse(Storage &in) {
	res = RESPONSE_AREA_CAPTURE();
	// ���X�|���X���
	res.ParseResponse(in);
}
