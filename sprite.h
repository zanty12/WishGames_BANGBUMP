/*==============================================================================

	Title		: SPRITE
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _SPRITE_H_
#define _SPRITE_H_


/*------------------------------------------------------------------------------
* include??
------------------------------------------------------------------------------*/
#include "lib\dxlib.h"
#include "texture.h"
#include "graphical.h"
#include "lib/collider2d.h"

/*------------------------------------------------------------------------------
* define??
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern??
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype??
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* enum??
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class??
------------------------------------------------------------------------------*/
void InitSprite(void);
/// <summary>
/// テクスチャを描画する
/// </summary>
/// <param name="texNo">テクスチャ番号</param>
/// <param name="pos">座標</param>
/// <param name="rot">角度</param>
/// <param name="scale">拡縮</param>
/// <param name="color">色</param>
void DrawSprite(int texNo, Vector2 pos, float rot, Vector2 scale, Color color);
/// <summary>
/// テクスチャを描画する
/// </summary>
/// <param name="texNo">テクスチャ番号</param>
/// <param name="pos">座標</param>
/// <param name="rot">角度</param>
/// <param name="scale">拡縮</param>
/// <param name="color">色</param>
/// <param name="uv">UV値</param>
/// <param name="uvWidth">UVサイズ</param>
void DrawSprite(int texNo, Vector2 pos, float rot, Vector2 scale, Color color, Vector2 uv, Vector2 uvWidth);
/// <summary>
/// テクスチャを描画する
/// </summary>
/// <param name="texNo">テクスチャ番号</param>
/// <param name="pos">座標</param>
/// <param name="rot">角度</param>
/// <param name="scale">拡縮</param>
/// <param name="color">色</param>
/// <param name="uv">UV値</param>
/// <param name="uvWidth">UVサイズ</param>
/// <param name="ratio">円</param>
void DrawSpriteCircleEffect(int texNo, Vector2 pos, float rot, Vector2 scale, Color color, Vector2 uv, Vector2 uvWidth, float ratio);
/// <summary>
/// テクスチャを描画する（左上が原点）
/// </summary>
/// <param name="texNo">テクスチャ番号</param>
/// <param name="pos">座標</param>
/// <param name="rot">角度</param>
/// <param name="scale">拡縮</param>
/// <param name="color">色</param>
void DrawSpriteLeftTop(int texNo, Vector2 pos, float rot, Vector2 scale, Color color);
/// <summary>
/// テクスチャを描画する（左上が原点）
/// </summary>
/// <param name="texNo">テクスチャ番号</param>
/// <param name="pos">座標</param>
/// <param name="rot">角度</param>
/// <param name="scale">拡縮</param>
/// <param name="color">色</param>
/// <param name="uv">UV値</param>
/// <param name="uvWidth">UVサイズ</param>
void DrawSpriteLeftTop(int texNo, Vector2 pos, float rot, Vector2 scale, Color color, Vector2 uv, Vector2 uvWidth);
/// <summary>
/// テクスチャを描画する（中心が原点）
/// </summary>
/// <param name="texNo">テクスチャ番号</param>
/// <param name="pos">座標</param>
/// <param name="rot">角度</param>
/// <param name="scale">拡縮</param>
/// <param name="color">色</param>
void DrawSpriteCenter(int texNo, Vector2 pos, float rot, Vector2 scale, Color color);
void DrawSprite(int texNo, Vector2 *vertices, Vector2 *uvs, Color color);
/// <summary>
/// UIを描画する（円形のUI）
/// </summary>
void DrawUICircle(int texNo, Vector2 pos, float rot, Vector2 scale, Color color, Vector2 uv, Vector2 uvWidth, float ratio);
void DrawUISquare(int texNo, Vector2 pos, float rot, Vector2 scale, Color color, float ratio);
void ReleaseSprite(void);

void DrawLine(Vector2 startPosition, Vector2 endPosition, Color color, float width = 10);
void DrawCollider(PHYSICS::Vertex1 vertex, Color color, Vector2 offset, float width = 10.0f);
void DrawCollider(PHYSICS::Vertex2 vertex, Color color, Vector2 offset, float width = 10.0f);
void DrawCollider(PHYSICS::Vertex4 vertex, Color color, Vector2 offset, float width = 10.0f);
void DrawCollider(PHYSICS::VertexN vertex, Color color, Vector2 offset, float width = 10.0f);

void DrawResource(ID3D11ShaderResourceView* texture, Vector2 pos, Vector2 scale,Color color);

void ViewUpdate(Vector2 pos);
void ProjectionUpdate(int screenWidth, int screenHeight);

#endif
