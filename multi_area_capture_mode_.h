#pragma once
#include "multi_mode.h"
#include "sprite.h"
#include "time.h"


class MultiPlayAreaCaptureModeServerSide : public MultiPlayServerSide {
public:
	struct Area : public RESPONSE_AREA_CAPTURE::DESC {
		float captureMaxTime = 5.0f;				// 占領の最大時間
		float captureNowTime = 0.0f;				// 占領時間
	};

private:
	std::list<Area> areas;
	MapMngr map_ = MapMngr("data/map/1.csv", nullptr);





private:
	// 占領の更新
	void CaptureUpdate(std::list<CLIENT_DATA_SERVER_SIDE>& clients) {
		using namespace PHYSICS;

		// エリア
		for (auto& area : areas) {
			Vertex1 areaCollider = Vertex1(area.position, area.radius);	// エリアのコライダー
			int touchID = -1;													// エリアに触れているID

			// プレイヤー
			for (auto& client : clients) {
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
				area.captureNowTime = 0.0f;
			}

			area.captureRatio = area.captureNowTime / area.captureMaxTime;		// 割合計算
		}
	}
	// 占領削除
	void DestoryUpdate() {
		for (auto iterator = areas.begin(); iterator == areas.end(); iterator++) {
			if (1.0f < iterator->captureRatio) {
				auto iterator__ = iterator;
				iterator__--;
				areas.erase(iterator);
				iterator = iterator__;
			}
		}
	}
	// スポーン
	void Spawn(Vector2 position) {
		// エリアの生成
		Area area;
		area.position = position;
		areas.push_back(area);
	}
public:
	MultiPlayAreaCaptureModeServerSide(MapMngr* map) : MultiPlayServerSide(map) {
		Spawn(Vector2(100, 100));
	}

	void Update(std::list<CLIENT_DATA_SERVER_SIDE>& clients) override {
		CaptureUpdate(clients);
		DestoryUpdate();
	}

	void CreateResponse(Storage &out) override {
		RESPONSE_AREA_CAPTURE res;
		// エリアの設定
		for (auto& area : areas)
			res.areas.push_back(area);

		// レスポンス作成
		res.CreateResponse(out);
	}
};






class MultiPlayAreaCaptureModeClientSide : public MultiPlayClientSide {
private:
	RESPONSE_AREA_CAPTURE res;
	int areaTexNo = -1;
	Animator areaAnim;
	MapMngr map_ = MapMngr("data/map/1.csv", nullptr);


public:
	MultiPlayAreaCaptureModeClientSide() : areaTexNo(LoadTexture("data/texture/area_capture.png")), areaAnim(Animator(Vector2::Zero, Vector2(100,100), areaTexNo)) { }

	void Draw(void) override {
		map_.Update();

		// エリアの描画
		for (auto area : res.areas) {
			areaAnim.SetPos(area.position);
			areaAnim.SetScale(Vector2(area.radius, area.radius));
			areaAnim.Draw();
		}
	}

	void ParseResponse(Storage& in) override {
		res = RESPONSE_AREA_CAPTURE();
		// レスポンス解析
		res.ParseResponse(in);
	}
};
