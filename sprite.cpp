#include "sprite.h"
#include "shader.h"


struct Vertex {
	Vector3 position;
	Vector2 uv;
};

DX::DX11::CORE::ConstantBuffer g_WorldBuffer;
DX::DX11::CORE::ConstantBuffer g_ViewBuffer;
DX::DX11::CORE::ConstantBuffer g_ProjectionBuffer;
DX::DX11::CORE::ConstantBuffer g_ColorBuffer;
DX::DX11::GRAPHICAL::Primitive<Vertex> g_Square;
DX::DX11::GRAPHICAL::Primitive<Vertex> g_Line;
DX::MATRIX g_WorldMatrix;
DX::MATRIX g_ViewMatrix;
DX::MATRIX g_ProjectionMatrix;



void InitSprite(void) {
	using namespace DX::DX11;
	ShaderManager::Initialize();


	Vertex v[] = {
		{ Vector3(-0.5f, +0.5f), Vector2(0.0f, 1.0f) },
		{ Vector3(+0.5f, +0.5f), Vector2(1.0f, 1.0f) },
		{ Vector3(-0.5f, -0.5f), Vector2(0.0f, 0.0f) },
		{ Vector3(+0.5f, -0.5f), Vector2(1.0f, 0.0f) },
	};
	// スプライトの作成
	g_Square.Create(v, 4, nullptr, 0, PRIMITIVE_SET_TYPE_COPY);
	g_Line.Create(v, 4, nullptr, 0, PRIMITIVE_SET_TYPE_COPY);

	// 定数バッファの作成
	g_WorldBuffer.Create(sizeof(DX::MATRIX), Device3D::Get());
	g_ViewBuffer.Create(sizeof(DX::MATRIX), Device3D::Get());
	g_ProjectionBuffer.Create(sizeof(DX::MATRIX), Device3D::Get());
	g_ColorBuffer.Create(sizeof(Color), Device3D::Get());
	Device3D::SetConstantBuffer(0, 1, g_WorldBuffer);
	Device3D::SetConstantBuffer(1, 1, g_ViewBuffer);
	Device3D::SetConstantBuffer(2, 1, g_ProjectionBuffer);
	Device3D::SetConstantBuffer(3, 1, g_ColorBuffer);

	ViewUpdate(Vector2::Zero);
	ProjectionUpdate(Graphical::GetWidth(), Graphical::GetHeight());
}

void DrawSprite(int texNo, Vector2 pos, float rot, Vector2 scale, Color color) {
	using namespace DX;
	using namespace DX::DX11;

	// トポロジの設定
	Device3D::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// テクスチャの設定
	auto texture = GetTexture(texNo);
	Device3D::SetResource(*GetTexture(texNo));

	// アフィン変換
	MATRIX translation, rotation, scaler, transform;
	pos.y = -pos.y + Graphical::GetHeight();
	translation.SetTranslation(pos);
	rotation.SetRotation(Vector3(0.0f, 0.0f, rot));
	scaler.SetScaling(scale);
	transform = scaler * rotation;
	transform = translation * transform;
	g_WorldMatrix = transform;	

	// シェーダーの設定
	ShaderManager::SetTextureMode();

	// 定数バッファの設定
	Device3D::UpdateConstantBuffer(&g_WorldMatrix, g_WorldBuffer);
	Device3D::UpdateConstantBuffer(&color, g_ColorBuffer);

	// 描画
	Device3D::Draw(
		g_Square.GetVertexBuffer(), g_Square.GetVertexCount(), g_Square.GetVertexStructByteSize(),
		g_Square.GetIndexBuffer(), g_Square.GetIndexCount(), g_Square.GetIndexStructByteSize()
	);
}

void DrawLine(Vector2 startPosition, Vector2 endPosition, Color color, float width) {
	using namespace DX;
	using namespace DX::DX11;

	// トポロジの設定
	Device3D::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 線の計算
	Vector2 direction = endPosition - startPosition;
	Vector2 normal = direction.Normal().Normalize();
	Vector2 centerLine = (startPosition + endPosition) / 2.0f;
	float rad = atan2f(direction.x, direction.y);
	float distance = direction.Distance();


	// アフィン変換
	MATRIX translation, rotation, scaler, transform;
	Vector2 pos, scale;
	pos += centerLine;
	pos.y = -pos.y + Graphical::GetHeight();
	scale = Vector2(width, distance);



	translation.SetTranslation(pos);
	rotation.SetRotation(Vector3(0.0f, 0.0f, rad));
	scaler.SetScaling(scale);
	transform = rotation * scaler;
	transform = translation * transform;
	g_WorldMatrix = transform;


	// シェーダーの設定
	ShaderManager::SetColorMode();

	// 定数バッファの設定
	Device3D::UpdateConstantBuffer(&g_WorldMatrix, g_WorldBuffer);
	Device3D::UpdateConstantBuffer(&color, g_ColorBuffer);

	// 描画
	Device3D::Draw(
		g_Square.GetVertexBuffer(), g_Square.GetVertexCount(), g_Square.GetVertexStructByteSize(),
		g_Square.GetIndexBuffer(), g_Square.GetIndexCount(), g_Square.GetIndexStructByteSize()
	);
}

void DrawCollider(PHYSICS::Vertex1 vertex, Color color, float width) {
	// 描画用の座標系に変換する
	const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
	const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
	vertex.a.x *= scale_x, vertex.a.y *= scale_y;
	Vector2 previous = Vector2(cosf(0.0f), sinf(0.0f)) * vertex.radius + vertex.a;

	for (float rad = 0.0f; rad < 6.28f; rad += MATH::Deg2Rad) {
		Vector2 circle = Vector2(cosf(rad), sinf(rad)) * vertex.radius + vertex.a;
		DrawLine(previous, circle, color, width);
		previous = circle;
	}
}

void DrawCollider(PHYSICS::Vertex2 vertex, Color color, float width) {
	// 描画用の座標系に変換する
	const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
	const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
	vertex.a.x *= scale_x, vertex.a.y *= scale_y;
	vertex.b.x *= scale_x, vertex.b.y *= scale_y;

	DrawLine(vertex.a, vertex.b, Color::Green, width);
}

void DrawCollider(PHYSICS::Vertex4 vertex, Color color, float width) {
	// 描画用の座標系に変換する
	const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
	const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
	vertex.a.x *= scale_x, vertex.a.y *= scale_y;
	vertex.b.x *= scale_x, vertex.b.y *= scale_y;
	vertex.c.x *= scale_x, vertex.c.y *= scale_y;
	vertex.d.x *= scale_x, vertex.d.y *= scale_y;

	DrawLine(vertex.a, vertex.b, Color::Green, width);
	DrawLine(vertex.b, vertex.c, Color::Green, width);
	DrawLine(vertex.c, vertex.d, Color::Green, width);
	DrawLine(vertex.d, vertex.a, Color::Green, width);
}

void DrawCollider(PHYSICS::VertexN vertex, Color color, float width) {
	// 描画用の座標系に変換する
	const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
	const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;

	if (vertex.num < 1) return;
	for (int i = 0; i < vertex.num - 1; i++) {
		Vector2 startPosition = vertex.v[i];
		Vector2 endPosition = vertex.v[(i + 1) % vertex.num];
		startPosition *= scale_x, startPosition.y *= scale_y;
		endPosition *= scale_x, endPosition.y *= scale_y;
		DrawLine(startPosition, endPosition, Color::Green, width);
	}
}




void ReleaseSprite(void) {
	g_Square.Release();
	ShaderManager::Release();
}

void ViewUpdate(Vector2 pos) {
	using namespace DX::DX11;
	// ビュー変換
	g_ViewMatrix.SetTranslation(-pos);
	Device3D::UpdateConstantBuffer(&g_ViewMatrix, g_ViewBuffer);
}

void ProjectionUpdate(int screenWidth, int screenHeight) {
	using namespace DX::DX11;
	// プロジェクション変換
	g_ProjectionMatrix.SetWorldViewProjection(screenWidth, screenHeight);
	Device3D::UpdateConstantBuffer(&g_ProjectionMatrix, g_ProjectionBuffer);
}
