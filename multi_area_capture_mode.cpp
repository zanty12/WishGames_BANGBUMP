#include "multi_area_capture_mode.h"
#include "ini.h"

/***********************************************************
	Server
************************************************************/
void MultiPlayAreaCaptureModeServerSide::ActiveArea(void) {
	if (areas.size() == 0) return;		// アクティブにするエリアがないなら終了

	auto iterator = areas.begin();

	// 更新する
	iterator->captureMaxTime = areaGetTime;
	iterator->radius = radius;

	activeAreas.push_back(*iterator);	// エリアをアクティブにする
	areas.erase(iterator);				// エリアからは削除する
}

void MultiPlayAreaCaptureModeServerSide::ActiveUpdate(void) {
	if (activeAreas.size()) return;		// アクティブなエリアがまだあるなら終了

	ActiveArea();
}

void MultiPlayAreaCaptureModeServerSide::CaptureUpdate(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	using namespace PHYSICS;

	// エリア
	for (auto &area : activeAreas) {
		Vertex1 areaCollider = Vertex1(area.position, area.radius);			// エリアのコライダー
		//CLIENT_DATA_SERVER_SIDE *pTouchClient = nullptr;					// エリアに触れているプレイヤー
		int inPlayerNum = 0;												// 領域に入っているプレイヤーの数

		int loop = 0;
		// プレイヤー
		for (auto &kvp : clients) {
			auto &client = kvp.second;
			auto &player = client.player_;
			auto touchIterator = inOrder.end();
			Vertex1 clientCollider = Vertex1(player->transform.position, 0.0f);	// プレイヤーのコライダー

			// すでにエリアに含まれているか
			for (touchIterator = inOrder.begin(); touchIterator != inOrder.end(); touchIterator++) {
				if (*touchIterator == player->id) {
					break;
				}
			}

			// 衝突判定なら入った順にID登録
			if (Collider2D::Touch(areaCollider, clientCollider)) {
				// ID登録
				if (touchIterator == inOrder.end()) inOrder.push_back(player->id);
				//int id = client.header.id;									// プレイヤーのID
				//inPlayerNum++;

				//
				//if (area.id == id ||	// 自分のエリアなら
				//	area.id == -1 ||	// だれのエリアでもないなら
				//	pTouchClient == nullptr) {	// 誰もいないなら
				//	pTouchClient = &client;		// 更新する
				//	area.id = id;
				//}
			}
			// エリアから離れたなら消す
			else if (touchIterator != inOrder.end())inOrder.erase(touchIterator);
		}

		// だれも触れていないなら
		//if (pTouchClient == nullptr) continue;

		// 占領し続けているならゲージの更新
		if (inOrder.size()) {
			area.captureNowTime += (curTime - preTime) * 0.001f;
			area.id = inOrder.front();

			area.captureRatio = area.captureNowTime / area.captureMaxTime;		// 割合計算

			// 占領完了したなら得点
			if (1.0f <= area.captureRatio) {
				clients[inOrder.front()].player_->score++;
				// スキルオーブをドロップさせる
				map_->DropSkillOrb(15, area.position, 10.0f);
			}
		}


	}
}
// 占領削除
void MultiPlayAreaCaptureModeServerSide::DestroyUpdate(void) {
	auto iterator = activeAreas.begin();

	while (iterator != activeAreas.end()) {
		// 占領完了
		if (1.0f < iterator->captureRatio) {
			// 削除
			auto tmp = iterator++;
			activeAreas.erase(tmp);
		}
		else iterator++;
	}
}

MultiPlayAreaCaptureModeServerSide::MultiPlayAreaCaptureModeServerSide()
	: MultiPlayModeServerSide(L"AreaCapture") {
	// 占領データを取得
	for (auto &areaPosition : GetMap()->GetAreaCaptures()) {
		areas.push_back(Area(areaPosition));
	}

	radius = ini::GetFloat(PARAM_PATH + L"mode.ini", L"AreaCapture", L"radius");
	areaGetTime = ini::GetFloat(PARAM_PATH + L"mode.ini", L"AreaCapture", L"areaGetTime");
	curTime = preTime = timeGetTime();
}

// ゲームアップデート
void MultiPlayAreaCaptureModeServerSide::Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	curTime = timeGetTime();
	ActiveUpdate();
	CaptureUpdate(clients);
	DestroyUpdate();
	preTime = curTime;
}

void MultiPlayAreaCaptureModeServerSide::CreateResponse(Storage &out) {
	RESPONSE_AREA_CAPTURE res;
	// エリアの設定
	for (auto &area : activeAreas) {
		res.areas.push_back(area);
	}

	// レスポンス作成
	res.CreateResponse(out);
}







/***********************************************************
	Client
************************************************************/
void MultiPlayAreaCaptureModeClientSide::Draw(RESPONSE_PLAYER &players, Vector2 offset) {

	// エリアの描画
	for (auto &area : res.areas) {
		Vector2 pos = area.position - offset;
		float rot = 0.0f;
		Vector2 scl = Vector2(area.radius, area.radius) * 2.0f;
		Color col = Color::White * 0.5f;
		anim.Draw(pos, rot, scl, col);
		DrawSpriteCircleEffect(anim.texNo, pos, rot, scl, col, anim.ToUV(), Vector2(1.0f / 5, 1.0f / 6), area.captureRatio);
	}
}

void MultiPlayAreaCaptureModeClientSide::ParseResponse(Storage &in) {
	res = RESPONSE_AREA_CAPTURE();
	// レスポンス解析
	res.ParseResponse(in);
}
