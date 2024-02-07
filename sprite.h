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
/// �e�N�X�`����`�悷��
/// </summary>
/// <param name="texNo">�e�N�X�`���ԍ�</param>
/// <param name="pos">���W</param>
/// <param name="rot">�p�x</param>
/// <param name="scale">�g�k</param>
/// <param name="color">�F</param>
void DrawSprite(int texNo, Vector2 pos, float rot, Vector2 scale, Color color);
/// <summary>
/// �e�N�X�`����`�悷��
/// </summary>
/// <param name="texNo">�e�N�X�`���ԍ�</param>
/// <param name="pos">���W</param>
/// <param name="rot">�p�x</param>
/// <param name="scale">�g�k</param>
/// <param name="color">�F</param>
/// <param name="uv">UV�l</param>
/// <param name="uvWidth">UV�T�C�Y</param>
void DrawSprite(int texNo, Vector2 pos, float rot, Vector2 scale, Color color, Vector2 uv, Vector2 uvWidth);
/// <summary>
/// �e�N�X�`����`�悷��
/// </summary>
/// <param name="texNo">�e�N�X�`���ԍ�</param>
/// <param name="pos">���W</param>
/// <param name="rot">�p�x</param>
/// <param name="scale">�g�k</param>
/// <param name="color">�F</param>
/// <param name="uv">UV�l</param>
/// <param name="uvWidth">UV�T�C�Y</param>
/// <param name="ratio">�~</param>
void DrawSpriteCircleEffect(int texNo, Vector2 pos, float rot, Vector2 scale, Color color, Vector2 uv, Vector2 uvWidth, float ratio);
/// <summary>
/// �e�N�X�`����`�悷��i���オ���_�j
/// </summary>
/// <param name="texNo">�e�N�X�`���ԍ�</param>
/// <param name="pos">���W</param>
/// <param name="rot">�p�x</param>
/// <param name="scale">�g�k</param>
/// <param name="color">�F</param>
void DrawSpriteLeftTop(int texNo, Vector2 pos, float rot, Vector2 scale, Color color);
/// <summary>
/// �e�N�X�`����`�悷��i���オ���_�j
/// </summary>
/// <param name="texNo">�e�N�X�`���ԍ�</param>
/// <param name="pos">���W</param>
/// <param name="rot">�p�x</param>
/// <param name="scale">�g�k</param>
/// <param name="color">�F</param>
/// <param name="uv">UV�l</param>
/// <param name="uvWidth">UV�T�C�Y</param>
void DrawSpriteLeftTop(int texNo, Vector2 pos, float rot, Vector2 scale, Color color, Vector2 uv, Vector2 uvWidth);
/// <summary>
/// �e�N�X�`����`�悷��i���S�����_�j
/// </summary>
/// <param name="texNo">�e�N�X�`���ԍ�</param>
/// <param name="pos">���W</param>
/// <param name="rot">�p�x</param>
/// <param name="scale">�g�k</param>
/// <param name="color">�F</param>
void DrawSpriteCenter(int texNo, Vector2 pos, float rot, Vector2 scale, Color color);
void DrawSprite(int texNo, Vector2 *vertices, Vector2 *uvs, Color color);
/// <summary>
/// UI��`�悷��i�~�`��UI�j
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
