#include "multi_area_capture_mode.h"

void MultiPlayAreaCaptureModeServerSide::ActiveArea(void) {
	if (areas.size() == 0) return;		// アクティブにするエリアがないなら終了

	auto iterator = areas.begin();
	activeAreas.push_back(*iterator);	// エリアをアクティブにする
	areas.erase(iterator);				// エリアからは削除する
}

void MultiPlayAreaCaptureModeServerSide::ActiveUpdate(void) {
	if (activeAreas.size()) return;		// アクティブなエリアがまだあるなら終了

	ActiveArea();
}

void MultiPlayAreaCaptureModeServerSide::CaptureUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
	using namespace PHYSICS;

	// エリア
	for (auto &area : activeAreas) {
		Vertex1 areaCollider = Vertex1(area.position, area.radius * 0.5f);	// エリアのコライダー
		int touchID = -1;													// エリアに触れているID

		// プレイヤー
		for (auto &client : clients) {
			Vertex1 clientCollider = Vertex1(client.player_->GetPos(), 1);	// プレイヤーのコライダー

			// 衝突判定
			if (Collider2D::Touch(areaCollider, clientCollider)) {
				int id = client.header.id;									// プレイヤーのID

				if (area.id == id ||	// 自分のエリアなら
					area.id == -1 ||	// だれのエリアでもないなら
					touchID == -1) {	// touchIDが初期値なら
					touchID = id;			// 更新する
				}

				// 誰も占領していないなら占領状態にする
				if (area.id == -1)
					area.id = client.header.id;

			}
		}

		// だれも触れていないなら
		if (touchID == -1) continue;

		// 占領し続けているならゲージの更新
		if (area.id == touchID) {
			area.captureNowTime += Time::GetDeltaTime();
		}
		// 別のプレイヤーに占領されたなら初期化
		else {
			area.id = touchID;
		}

		area.captureRatio = area.captureNowTime / area.captureMaxTime;		// 割合計算
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
// スポーン
void MultiPlayAreaCaptureModeServerSide::Spawn(Vector2 position) {
	// エリアの生成
	Area area;
	area.position = position;
	areas.push_back(area);
}



MultiPlayAreaCaptureModeServerSide::MultiPlayAreaCaptureModeServerSide(MapMngr *map_) : MultiPlayServerSide(map_) {
	int WIDTH = map_->GetMap()->GetWidth();					// 幅
	int HEIGHT = map_->GetMap()->GetHeight();				// 高さ

	// マップを取得
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {

			// セルを取得
			Cell *cell = map_->GetMap()->GetCell(x, y);
			if (cell == nullptr) continue;

			// エリアなら
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
	// エリアの設定
	for (auto &area : activeAreas)
		res.areas.push_back(area);

	// レスポンス作成
	res.CreateResponse(out);
}







void MultiPlayAreaCaptureModeClientSide::Draw(void) {
	// エリアの描画
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
	// レスポンス解析
	res.ParseResponse(in);
}
