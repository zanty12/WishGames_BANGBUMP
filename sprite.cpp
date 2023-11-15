#include "sprite.h"
#include "camera.h"
#include "shader.h"


struct Vertex {
	Vector3 position;
	Vector2 texcoord;
	Color color;
};

DX::DX11::CORE::ConstantBuffer g_WorldBuffer;
DX::DX11::CORE::ConstantBuffer g_ColorBuffer;
DX::DX11::GRAPHICAL::Primitive<Vertex> g_Square;

extern DX::DX11::CORE::ConstantBuffer g_ViewBuffer;
extern DX::DX11::CORE::ConstantBuffer g_ProjectionBuffer;
DX::MATRIX g_WorldMatrix;



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

	CameraMove(Vector2::Zero);
	ProjectionUpdate();
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
	translation.SetTranslation(Vector3(pos.x, -pos.y, 0.0f));
	rotation.SetRotation(Vector3(0.0f, 0.0f, rot));
	scaler.SetScaling(scale);
	transform = scaler * rotation;
	transform = translation * transform;
	g_WorldMatrix = transform;

	// 定数バッファの設定
	Device3D::UpdateConstantBuffer(&g_WorldMatrix, g_WorldBuffer);
	Device3D::UpdateConstantBuffer(&color, g_ColorBuffer);
	// シェーダーの設定
	ShaderManager::SetTextureMode();
	// 描画
	Device3D::Draw(
		g_Square.GetVertexBuffer(), g_Square.GetVertexCount(), g_Square.GetVertexStructByteSize(),
		g_Square.GetIndexBuffer(), g_Square.GetIndexCount(), g_Square.GetIndexStructByteSize()
	);
}

void DrawLine(Vector2 startPosition, Vector2 endPosition) {

}



void ReleaseSprite(void) {
	g_Square.Release();
	ShaderManager::Release();
}
