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
		CLIENT_DATA_SERVER_SIDE *pTouchClient = nullptr;					// エリアに触れているプレイヤー
		int inPlayerNum = 0;												// 領域に入っているプレイヤーの数


		// プレイヤー
		for (auto &kvp : clients) {
			auto &client = kvp.second;
			auto &player = client.player_;
			Vertex1 clientCollider = Vertex1(player->transform.position, 0.0f);	// プレイヤーのコライダー

			// 衝突判定
			if (Collider2D::Touch(areaCollider, clientCollider)) {
				int id = client.header.id;									// プレイヤーのID
				inPlayerNum++;

				if (area.id == id ||	// 自分のエリアなら
					area.id == -1 ||	// だれのエリアでもないなら
					pTouchClient == nullptr) {	// 誰もいないなら
					pTouchClient = &client;		// 更新する
					area.id = id;
				}
			}
		}

		// だれも触れていないなら
		if (pTouchClient == nullptr) continue;

		// 占領し続けているならゲージの更新
		if (area.id == pTouchClient->header.id) {
			area.captureNowTime += (curTime - preTime) * 0.001f;
		}
		// 別のプレイヤーに占領されたなら初期化
		else {
			area.id = pTouchClient->header.id;
		}

		area.captureRatio = area.captureNowTime / area.captureMaxTime;		// 割合計算

		// 占領完了したなら得点
		if (1.0f <= area.captureRatio) {
			pTouchClient->player_->score++;
			// スキルオーブをドロップさせる
			map_->DropSkillOrb(15, area.position, 10.0f);
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
	: MultiPlayModeServerSide(new MultiMap(MAP_PATH + "MultiPlay_Map1.csv", MULTIPLAY_RUN_TYPE_SERVER), L"AreaCapture") {
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
		DrawSpriteCircleEffect(anim.texNo, pos, rot, scl, col, Vector2::Zero, Vector2::One, area.captureRatio);
		anim.SetActive(false);
	}
}

void MultiPlayAreaCaptureModeClientSide::ParseResponse(Storage &in) {
	res = RESPONSE_AREA_CAPTURE();
	// レスポンス解析
	res.ParseResponse(in);
}
