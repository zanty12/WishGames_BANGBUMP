#include "camera.h"
#include "graphical.h"
#include "lib/dxlib.h"

DX::DX11::CORE::ConstantBuffer g_ViewBuffer;
DX::DX11::CORE::ConstantBuffer g_ProjectionBuffer;
DX::MATRIX g_ViewMatrix;
DX::MATRIX g_ProjectionMatrix;


void CameraMove(Vector2 position) {
	using namespace DX::DX11;
	Vector2 size = Graphical::GetHwnd().GetSize();
	size.x = 0;
	// ビュー変換
	g_ViewMatrix.SetTranslation(-position + size);
	Device3D::UpdateConstantBuffer(&g_ViewMatrix, g_ViewBuffer);
}

void ProjectionUpdate(void) {
	using namespace DX::DX11;
	// プロジェクション変換
	Vector2 size = Graphical::GetHwnd().GetSize();
	g_ProjectionMatrix.SetWorldViewProjection(size.x, size.y);
	Device3D::UpdateConstantBuffer(&g_ProjectionMatrix, g_ProjectionBuffer);
}
