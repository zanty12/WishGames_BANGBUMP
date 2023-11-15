/*==============================================================================

	Title		: VERTEX_T
	Description : 頂点構造体
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_VERTEX_T_H_
#define _DX11_VERTEX_T_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include "stdlib.h"

/*------------------------------------------------------------------------------
* define宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* struct宣言
------------------------------------------------------------------------------*/
namespace DX {
	namespace DX11 {
		/// <summary>
		/// 3D頂点構造体
		/// </summary>
		struct VERTEX_T {
			Vector3 position = Vector3::Zero;
			Vector3 normal = Vector3::Zero;
			Color color = Color::White;
			Vector2 texcoord = Vector2::Zero;

			VERTEX_T(Vector3 position) : position(position) { }
			VERTEX_T(Vector3 position, Color color) : position(position), color(color) { }
			VERTEX_T(Vector3 position, Vector2 texcoord) : position(position), texcoord(texcoord) { }
			VERTEX_T(Vector3 position, Color color, Vector2 texcoord) : position(position), color(color), texcoord(texcoord) { }
		};
	};
};

#endif
