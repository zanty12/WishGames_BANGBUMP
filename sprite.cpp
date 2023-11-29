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
