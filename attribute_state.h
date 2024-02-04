#pragma once
#include <string>
#include "multi_path.h"
#include "ini.h"

#define influence_state(val) val = state.val < 0.0f ? val : state.val
struct AttributeState {
	// �p���[
	float minPower = 0.0f;			// �p���[�̍ŏ��l
	float maxPower = 10.0f;			// �p���[�̍ő�l
	float addPower = 00.0f;			// �p���[�̉��Z�l
	float friction = 0.98f;			// ���C�W��
	float brakeFriction = 0.98f;	// ���C�W���i�}��~�j
	float powerFriction = 1.00f;	// �p���[�̖��C�W��

	// �؂�ւ�
	float exCoolTime = 1.5f;		// ��������񂱃N�[���^�C��

	// ����
	int minMp = 0;					// �ŏ�����
	int maxMp = 3;					// �ő喂��
	int cost = 1;					// ���͏����
	float healSpanTime = 1.0f;		// �񕜂��鎞��
	int heal = 1;					// �񕜗�

	// ����
	float minInputDistance = 0.50f;	// ���͂̔���l
	float minInputSpeed = 0.50f;	// ���͂̉����̔���l
	float inputPowerRate = 1.0f;	// ���͒l���p���[�ɗ^����W��
	float minActionPower = 0.0f;	// ��������������p���[�̍ŏ��l

	// �U��
	float atkRange = 1.0f;			// �U���͈�
	float atkDistance = 10.0f;		// �U������
	int atk = 0;					// �U����	
	int atkDrop = 0;				// �h���b�v��
	float coolTime = 0.0f;			// �N�[���^�C��
	float atkCoolTime = 1.0f;		// �U���N�[���^�C��
	float atkAfterTime = 0.0f;		// �U���̎c���
	float knockbackRate = 10.0f;	// �m�b�N�o�b�N��

	// �\��
	float showMoveX = 100.0f;
	float showMoveY = 100.0f;
	float showAttackX = 100.0f;
	float showAttackY = 100.0f;

	AttributeState() = default;
	AttributeState(std::wstring attributeName, int lv) {
		std::wstring path = L"attribute_lv" + std::to_wstring(lv) + L".ini";
		// �p���[
		minPower = ini::GetFloat(PARAM_PATH + path, attributeName, L"minPower", -1.0f);
		maxPower = ini::GetFloat(PARAM_PATH + path, attributeName, L"maxPower", -1.0f);
		addPower = ini::GetFloat(PARAM_PATH + path, attributeName, L"addPower", -1.0f);
		friction = ini::GetFloat(PARAM_PATH + path, attributeName, L"friction", -1.0f);
		brakeFriction = ini::GetFloat(PARAM_PATH + path, attributeName, L"brakeFriction", -1.0f);
		powerFriction = ini::GetFloat(PARAM_PATH + path, attributeName, L"powerFriction", -1.0f);

		// �؂�ւ�
		exCoolTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"exCoolTime", -1.0f);

		// ����
		minMp = ini::GetInt(PARAM_PATH + path, attributeName, L"minMp", -1);
		maxMp = ini::GetInt(PARAM_PATH + path, attributeName, L"maxMp", -1);
		cost = ini::GetInt(PARAM_PATH + path, attributeName, L"cost", -1);
		healSpanTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"healSpanTime", -1.0f);
		heal = ini::GetInt(PARAM_PATH + path, attributeName, L"heal", -1);

		// ����
		minInputDistance = ini::GetFloat(PARAM_PATH + path, attributeName, L"minInputDistance", -1.0f);
		minInputSpeed = ini::GetFloat(PARAM_PATH + path, attributeName, L"minInputSpeed", -1.0f);
		inputPowerRate = ini::GetFloat(PARAM_PATH + path, attributeName, L"inputPowerRate", -1.0f);
		minActionPower = ini::GetFloat(PARAM_PATH + path, attributeName, L"minActionPower", -1.0f);

		// �U��
		atk = ini::GetFloat(PARAM_PATH + path, attributeName, L"atk", -1.0f);
		atkDrop = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkDrop", -1.0f);
		atkCoolTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkCoolTime", -1.0f);
		atkAfterTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkAfterTime", -1.0f);
		coolTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"coolTime", -1.0f);
		atkRange = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkRange", -1.0f);
		atkDistance = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkDistance", -1.0f);
		knockbackRate = ini::GetFloat(PARAM_PATH + path, attributeName, L"knockbackRate", -1.0f);

		// �\��
		showMoveX = ini::GetFloat(PARAM_PATH + path, attributeName, L"showMoveX", -1.0f);
		showMoveY = ini::GetFloat(PARAM_PATH + path, attributeName, L"showMoveY", -1.0f);
		showAttackX = ini::GetFloat(PARAM_PATH + path, attributeName, L"showAttackX", -1.0f);
		showAttackY = ini::GetFloat(PARAM_PATH + path, attributeName, L"showAttackY", -1.0f);
	}

	void operator << (AttributeState &state) {
		// �p���[
		influence_state(minPower);
		influence_state(maxPower);
		influence_state(addPower);
		influence_state(friction);
		influence_state(brakeFriction);
		influence_state(powerFriction);

		// �؂�ւ�
		influence_state(exCoolTime);

		// ����
		influence_state(minMp);
		influence_state(maxMp);
		influence_state(cost);
		influence_state(healSpanTime);
		influence_state(heal);

		// ����
		influence_state(minInputDistance);
		influence_state(minInputSpeed);
		influence_state(inputPowerRate);
		influence_state(minActionPower);

		// �U��
		influence_state(atk);
		influence_state(atkDrop);
		influence_state(atkCoolTime);
		influence_state(atkAfterTime);
		influence_state(coolTime);
		influence_state(atkRange);
		influence_state(atkDistance);
		influence_state(knockbackRate);

		// �\��
		influence_state(showMoveX);
		influence_state(showMoveY);
		influence_state(showAttackX);
		influence_state(showAttackY);
	}

private:

};
