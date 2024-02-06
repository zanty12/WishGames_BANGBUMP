#pragma once
#include "multi_transform.h"
#include "multi_behavior.h"
#include "multi_objenum.h"

class MultiMap;
class GameObjectServerSide : public MultiBehavior {
public:
	static int MAX_ID;

	Transform transform;
	Vector2 velocity;
	int damageEffectAttributeType = -1;					// ダメージ与えられた時の属性タイプ
	float gravity = 0.0f;
	float radius = 10.0f;
	int id = MAX_ID++;

	GameObjectServerSide() = default;
	GameObjectServerSide(Transform transform) : transform(transform) { }
	virtual MULTI_OBJECT_TYPE GetType(void) = 0;
};

class GameObjectClientSide : public MultiBehavior {
public:
	Transform transform;
	Vector2 velocity;
	int damageEffectAttributeType = -1;					// ダメージ与えられた時の属性タイプ
	float radius = 10.0f;
	int texNo = -1;
	int id = 0;
	bool isShow = true;
	bool isPrevShow = true;


	GameObjectClientSide() = default;
	GameObjectClientSide(Transform transform) : transform(transform) { };
	virtual void Release(void) { };
};