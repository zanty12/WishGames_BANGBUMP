#include "multi_area_capture_mode.h"

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

void MultiPlayAreaCaptureModeServerSide::CaptureUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
	using namespace PHYSICS;

	// �G���A
	for (auto &area : activeAreas) {
		Vertex1 areaCollider = Vertex1(area.position, area.radius * 0.5f);	// �G���A�̃R���C�_�[
		int touchID = -1;													// �G���A�ɐG��Ă���ID

		// �v���C���[
		for (auto &client : clients) {
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
		}

		area.captureRatio = area.captureNowTime / area.captureMaxTime;		// �����v�Z
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
// �X�|�[��
void MultiPlayAreaCaptureModeServerSide::Spawn(Vector2 position) {
	// �G���A�̐���
	Area area;
	area.position = position;
	areas.push_back(area);
}



MultiPlayAreaCaptureModeServerSide::MultiPlayAreaCaptureModeServerSide(MapMngr *map_) : MultiPlayServerSide(map_) {
	int WIDTH = map_->GetMap()->GetWidth();					// ��
	int HEIGHT = map_->GetMap()->GetHeight();				// ����

	// �}�b�v���擾
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {

			// �Z�����擾
			Cell *cell = map_->GetMap()->GetCell(x, y);
			if (cell == nullptr) continue;

			// �G���A�Ȃ�
			if (cell->GetCellType() == MAP_READ_MULTI_AREA_CAPTURE) {
				Spawn(cell->GetPos());
			}
		}
	}
}

void MultiPlayAreaCaptureModeServerSide::Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
	ActiveUpdate();
	CaptureUpdate(clients);
	DestroyUpdate();
}

void MultiPlayAreaCaptureModeServerSide::CreateResponse(Storage &out) {
	RESPONSE_AREA_CAPTURE res;
	// �G���A�̐ݒ�
	for (auto &area : activeAreas)
		res.areas.push_back(area);

	// ���X�|���X�쐬
	res.CreateResponse(out);
}







void MultiPlayAreaCaptureModeClientSide::Draw(void) {
	// �G���A�̕`��
	for (auto &area : res.areas) {
		anim.SetTexNo(areaTexNo);
		anim.SetPos(area.position);
		anim.SetScale(Vector2(area.radius, area.radius));
		anim.Draw();
	}
	anim.SetTexNo(-1);
}

void MultiPlayAreaCaptureModeClientSide::ParseResponse(Storage &in) {
	res = RESPONSE_AREA_CAPTURE();
	// ���X�|���X���
	res.ParseResponse(in);
}
