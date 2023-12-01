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
* include�錾
------------------------------------------------------------------------------*/
#include "lib\dxlib.h"
#include "texture.h"
#include "graphical.h"
#include "lib/collider2d.h"

/*------------------------------------------------------------------------------
* define�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* enum�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class�錾
------------------------------------------------------------------------------*/
void InitSprite(void);
void DrawSprite(int texNo, Vector2 pos, float rot, Vector2 scale, Color color);
void ReleaseSprite(void);

void DrawLine(Vector2 startPosition, Vector2 endPosition, Color color, float width = 10);
void DrawCollider(PHYSICS::Vertex1 vertex, Color color, float width = 10.0f);
void DrawCollider(PHYSICS::Vertex2 vertex, Color color, float width = 10.0f);
void DrawCollider(PHYSICS::Vertex4 vertex, Color color, float width = 10.0f);
void DrawCollider(PHYSICS::VertexN vertex, Color color, float width = 10.0f);

void ViewUpdate(Vector2 pos);
void ProjectionUpdate(int screenWidth, int screenHeight);

#endif
