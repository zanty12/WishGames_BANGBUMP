#pragma once
#include <string>
#include "multi_path.h"
#include "ini.h"

#define influence_state(val) val = state.val < 0.0f ? val : state.val
struct AttributeState {
	// パワー
	float minPower = 0.0f;			// パワーの最小値
	float maxPower = 10.0f;			// パワーの最大値
	float addPower = 00.0f;			// パワーの加算値
	float friction = 0.98f;			// 摩擦係数
	float brakeFriction = 0.98f;	// 摩擦係数（急停止）
	float powerFriction = 1.00f;	// パワーの摩擦係数

	// 切り替え
	float exCoolTime = 1.5f;		// がっちゃんこクールタイム

	// 魔力
	int minMp = 0;					// 最小魔力
	int maxMp = 3;					// 最大魔力
	int cost = 1;					// 魔力消費量
	float healSpanTime = 1.0f;		// 回復する時間
	int heal = 1;					// 回復量

	// 入力
	float minInputDistance = 0.50f;	// 入力の判定値
	float minInputSpeed = 0.50f;	// 入力の加速の判定値
	float inputPowerRate = 1.0f;	// 入力値がパワーに与える係数
	float minActionPower = 0.0f;	// 属性が発動するパワーの最小値

	// 攻撃
	float atkRange = 1.0f;			// 攻撃範囲
	float atkDistance = 10.0f;		// 攻撃長さ
	int atk = 0;					// 攻撃力	
	int atkDrop = 0;				// ドロップ量
	float coolTime = 0.0f;			// クールタイム
	float atkCoolTime = 1.0f;		// 攻撃クールタイム
	float atkAfterTime = 0.0f;		// 攻撃の残り火
	float knockbackRate = 10.0f;	// ノックバック量

	// 表示
	float showMoveX = 100.0f;
	float showMoveY = 100.0f;
	float showAttackX = 100.0f;
	float showAttackY = 100.0f;

	AttributeState() = default;
	AttributeState(std::wstring attributeName, int lv) {
		std::wstring path = L"attribute_lv" + std::to_wstring(lv) + L".ini";
		// パワー
		minPower = ini::GetFloat(PARAM_PATH + path, attributeName, L"minPower", -1.0f);
		maxPower = ini::GetFloat(PARAM_PATH + path, attributeName, L"maxPower", -1.0f);
		addPower = ini::GetFloat(PARAM_PATH + path, attributeName, L"addPower", -1.0f);
		friction = ini::GetFloat(PARAM_PATH + path, attributeName, L"friction", -1.0f);
		brakeFriction = ini::GetFloat(PARAM_PATH + path, attributeName, L"brakeFriction", -1.0f);
		powerFriction = ini::GetFloat(PARAM_PATH + path, attributeName, L"powerFriction", -1.0f);

		// 切り替え
		exCoolTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"exCoolTime", -1.0f);

		// 魔力
		minMp = ini::GetInt(PARAM_PATH + path, attributeName, L"minMp", -1);
		maxMp = ini::GetInt(PARAM_PATH + path, attributeName, L"maxMp", -1);
		cost = ini::GetInt(PARAM_PATH + path, attributeName, L"cost", -1);
		healSpanTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"healSpanTime", -1.0f);
		heal = ini::GetInt(PARAM_PATH + path, attributeName, L"heal", -1);

		// 入力
		minInputDistance = ini::GetFloat(PARAM_PATH + path, attributeName, L"minInputDistance", -1.0f);
		minInputSpeed = ini::GetFloat(PARAM_PATH + path, attributeName, L"minInputSpeed", -1.0f);
		inputPowerRate = ini::GetFloat(PARAM_PATH + path, attributeName, L"inputPowerRate", -1.0f);
		minActionPower = ini::GetFloat(PARAM_PATH + path, attributeName, L"minActionPower", -1.0f);

		// 攻撃
		atk = ini::GetFloat(PARAM_PATH + path, attributeName, L"atk", -1.0f);
		atkDrop = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkDrop", -1.0f);
		atkCoolTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkCoolTime", -1.0f);
		atkAfterTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkAfterTime", -1.0f);
		coolTime = ini::GetFloat(PARAM_PATH + path, attributeName, L"coolTime", -1.0f);
		atkRange = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkRange", -1.0f);
		atkDistance = ini::GetFloat(PARAM_PATH + path, attributeName, L"atkDistance", -1.0f);
		knockbackRate = ini::GetFloat(PARAM_PATH + path, attributeName, L"knockbackRate", -1.0f);

		// 表示
		showMoveX = ini::GetFloat(PARAM_PATH + path, attributeName, L"showMoveX", -1.0f);
		showMoveY = ini::GetFloat(PARAM_PATH + path, attributeName, L"showMoveY", -1.0f);
		showAttackX = ini::GetFloat(PARAM_PATH + path, attributeName, L"showAttackX", -1.0f);
		showAttackY = ini::GetFloat(PARAM_PATH + path, attributeName, L"showAttackY", -1.0f);
	}

	void operator << (AttributeState &state) {
		// パワー
		influence_state(minPower);
		influence_state(maxPower);
		influence_state(addPower);
		influence_state(friction);
		influence_state(brakeFriction);
		influence_state(powerFriction);

		// 切り替え
		influence_state(exCoolTime);

		// 魔力
		influence_state(minMp);
		influence_state(maxMp);
		influence_state(cost);
		influence_state(healSpanTime);
		influence_state(heal);

		// 入力
		influence_state(minInputDistance);
		influence_state(minInputSpeed);
		influence_state(inputPowerRate);
		influence_state(minActionPower);

		// 攻撃
		influence_state(atk);
		influence_state(atkDrop);
		influence_state(atkCoolTime);
		influence_state(atkAfterTime);
		influence_state(coolTime);
		influence_state(atkRange);
		influence_state(atkDistance);
		influence_state(knockbackRate);

		// 表示
		influence_state(showMoveX);
		influence_state(showMoveY);
		influence_state(showAttackX);
		influence_state(showAttackY);
	}

private:

};
