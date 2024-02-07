#pragma once
#include <sstream>
#include "lib/win_time.h"
#include "multi_movable_object.h"
#include "multi_attribute.h"
#include "multi_anim.h"
#include "multi_map.h"
#include "attribute_type.h"
#include "multi_animenum.h"
#include "multi_path.h"
#include "ini.h"
#define MAX_LV 10

/*******************************************************
  Server
********************************************************/
class ServerAttribute;
class AttackServerSide;
class ServerPlayer : public ServerMovableGameObject {
private:
	ServerAttribute *moveAttribute = nullptr;			// 移動属性
	ServerAttribute *attackAttribute = nullptr;			// 攻撃属性
	WIN::Time exCoolTime;								// がっちゃんこクールタイム


public:
	int skillPoint = 0;									// スキルポイント
	int score = 0;										// スコア
	int animType = ANIMATION_TYPE_IDLE;					// アニメーション
	MultiMap *map = nullptr;							// マップ
	Vector2 attackVelocity;								// 攻撃のベロシティ
	Vector2 chargeVelocity;								// チャージベロシティ
	bool isAttributeChange = false;						// 属性チェンジ
	WIN::Time exStartTime;								// がっちゃんこ開始タイマー

	int lvupPoint[MAX_LV] = {};							// レベルアップに必要なポイント

public:
	ServerPlayer() {
		std::wstring lvStr = L"lv";
		for (int i = 0; i < MAX_LV; i++) {
			lvupPoint[i] = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", lvStr + std::to_wstring(i + 1), 0);
		}

		radius = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"radius", 10.0f);
		gravity = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"gravity", 0.01f);
		maxGravity = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"maxGravity", 0.5f);
		transform.scale.x = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"width", 20.0f);
		transform.scale.y = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"height", 60.0f);
		exCoolTime.Start();
	}
	~ServerPlayer() {
		if (moveAttribute) delete moveAttribute;
		if (attackAttribute) delete attackAttribute;
		moveAttribute = nullptr;
		attackAttribute = nullptr;
	}
	void Damage(AttackServerSide *attack) override;
	void SkillOrbDrop(int drop);

	void SetMoveAttribute(ServerAttribute *moveAttribute) { this->moveAttribute = moveAttribute; }
	void SetAttackAttribute(ServerAttribute *attackAttribute) { this->attackAttribute = attackAttribute; }
	void SetAttribute(ServerAttribute *moveAttribute, ServerAttribute *attackAttribute) { this->moveAttribute = moveAttribute, this->attackAttribute = attackAttribute; }
	ServerAttribute *GetMoveAttribute(void) { return moveAttribute; }
	ServerAttribute *GetAttackAttribute(void) { return attackAttribute; }

	int GetLv(void) {
		int lv = MAX_LV - 1;
		// レベルが上限の場合
		if (lvupPoint[lv] <= skillPoint) {
			return lv;
		}

		// レベルの調整
		for (lv = 0; lv < MAX_LV - 1; lv++) {
			if (lvupPoint[lv] <= skillPoint && skillPoint < lvupPoint[lv + 1]) {
				return lv;
			}
		}
	}
	int GetLvMinSkillOrb(void) {
		int lv = GetLv();
		int min = lvupPoint[lv];
		return min;
	}
	int GetLvMaxSkillOrb(void) {
		int lv = GetLv();
		int max = lv < MAX_LV - 1 ? lvupPoint[lv + 1] : -1;
		return max;
	}
	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_PLAYER; }
};






/*******************************************************
  Client
********************************************************/
class ClientAttribute;
class AttackClientSide;
class ClientPlayer : public ClientMovableGameObject {
private:
	enum ENTRY_TYPE {
		NONE,
		ENTRY,
		SHOW,
		EXIT
	};

private:
	MultiAnimator anim;										// アニメーション
	MultiAnimator reverseAnim;								// 反転アニメーション
	MultiAnimator *curAnim = nullptr;						// アニメーション（現在）
	WIN::Time timer;										// スポーン開始時からの時間計測
	ENTRY_TYPE entryType = NONE;							// 入場演出


	MultiAnimator allDamageEffect;							// ダメージエフェクト
	MultiAnimator fireDamageEffect;							// 炎ダメージエフェクト
	MultiAnimator waterDamageEffect;						// 水ダメージエフェクト
	MultiAnimator thunderDamageEffect;						// 雷ダメージエフェクト
	MultiAnimator windDamageEffect;							// 風ダメージエフェクト
	MultiAnimator exEffect;									// がっちゃんこエフェクト
	MultiAnimator lvUpEffect;								// レベルアップエフェクト
	MultiAnimator lvDownEffect;								// レベルダウンエフェクト
	MultiAnimator lvUpUI;									// レベルアップUIエフェクト
	MultiAnimator lvDownUI;									// レベルダウンUIエフェクト

public:
	int skillPoint = 0;										// スキルポイント
	int score = 0;											// スコア
	int animType = ANIMATION_TYPE_IDLE;						// アニメーションタイプ
	int preAnimType = ANIMATION_TYPE_IDLE;					// アニメーション（1フレーム前）
	int lv = 0;												// レベル
	int preLv = 0;											// レベル（1フレーム前）
	ATTRIBUTE_TYPE moveAttributeType;						// 移動属性タイプ
	ATTRIBUTE_TYPE attackAttributeType;						// 攻撃属性タイプ
	ATTRIBUTE_TYPE preMoveAttributeType;					// 移動属性タイプ
	ATTRIBUTE_TYPE preAttackAttributeType;					// 攻撃属性タイプ
	bool isReverseXAttributeControl = false;				// 横軸の向きを属性側でコントロールするか否か
	bool isRotationAttributeControl = false;				// 向きを属性側でコントロールするか否か
	bool isReverseX = false;								// 横軸の向き
	Vector2 attackVelocity;									// 攻撃のベロシティ
	Vector2 chargeVelocity;									// チャージベロシティ
	ClientAttribute *moveAttribute = nullptr;				// 移動属性
	ClientAttribute *attackAttribute = nullptr;				// 攻撃属性
	ClientAttribute *curMoveAttribute = nullptr;			// 移動属性（現在）
	ClientAttribute *curAttackAttribute = nullptr;			// 攻撃属性（現在）
	float skillPointAnimation = 0.0f;						// スキルポイント獲得時のゲージのアニメーションで使用する
	int lvupPoint[MAX_LV] = {};								// レベルアップに必要なポイント


public:
	ClientPlayer(ATTRIBUTE_TYPE moveAttributeType, ATTRIBUTE_TYPE attackAttributeType, Transform transform);

	void Loop(void) override;
	void ShowEntry();
	void ShowExit();
	void DrawUI();

	void Update(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute, MultiAnimator *anim);
	void SetMoveAttribute(ClientAttribute *moveAttribute);
	void SetAttackAttribute(ClientAttribute *attackAttribute);
	void SetAttribute(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute) {
		SetMoveAttribute(moveAttribute);
		SetAttackAttribute(attackAttribute);
	}

	int GetLv(void) {
		int lv = MAX_LV - 1;
		// レベルが上限の場合
		if (lvupPoint[lv] <= skillPoint) {
			return lv;
		}

		// レベルの調整
		for (lv = 0; lv < MAX_LV - 1; lv++) {
			if (lvupPoint[lv] <= skillPoint && skillPoint < lvupPoint[lv + 1]) {
				return lv;
			}
		}
	}
	int GetLvMinSkillOrb(void) {
		int lv = GetLv();
		int min = lvupPoint[lv];
		return min;
	}
	int GetLvMaxSkillOrb(void) {
		int lv = GetLv();
		int max = lv < MAX_LV - 1 ? lvupPoint[lv + 1] : -1;
		return max;
	}
	ClientAttribute *GetMoveAttribute(void) { return moveAttribute; }
	ClientAttribute *GetAttackAttribute(void) { return attackAttribute; }
};

