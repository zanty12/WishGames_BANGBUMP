/*==============================================================================

	Title		: COLLIDER2D
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _COLLIDER2D_H_
#define _COLLIDER2D_H_


/*------------------------------------------------------------------------------
* includeéŒ¾
------------------------------------------------------------------------------*/
#include "vector.h"


/*------------------------------------------------------------------------------
* defineéŒ¾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* externéŒ¾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototypeéŒ¾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* enuméŒ¾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* classéŒ¾
------------------------------------------------------------------------------*/
namespace PHYSICS {
	class Collider2D;

	struct Vertex1 {
		Vector2 a;
		float radius = 0.0f;
		Vertex1(Vector2 a, float radius = 0.0f) : a(a), radius(radius) { }
	};
	struct Vertex2 {
	protected:
		friend Collider2D;
		Vector2 a_b;

	public:
		Vector2 a, b;
		Vertex2(Vector2 a, Vector2 b) : a(a), b(b), a_b(b - a) { }
		Vertex3 Translate(Vector2 pos) {
			a += pos;
			b += pos;
			a_b = b - a;
		}
		Vertex3 Rotate(float rad) {
			float s_ = sinf(rad);
			float c_ = cosf(rad);
			a.x = a.x * +c_ + a.y * -s_;
			a.y = a.x * +s_ + a.y * +c_;
			b.x = b.x * +c_ + b.y * -s_;
			b.y = b.x * +s_ + b.y * +c_;
			a_b = b - a;
		}
		Vertex3 Scale(Vector2 scl) {
			a.x *= scl.x;
			a.y *= scl.y;
			b.x *= scl.x;
			b.y *= scl.y;
			a_b = b - a;
		}
	};
	struct Vertex3 {
	protected:
		friend Collider2D;
		Vector2 a_b;
		Vector2 b_c;
		Vector2 c_a;


	public:
		union {
			struct {
				Vector2 a;
				Vector2 b;
				Vector2 c;
			};
			Vector2 v[3];
		};

		Vertex3(Vector2 a, Vector2 b, Vector2 c) : a(a), b(b), c(c), a_b(b - a), b_c(c - b), c_a(a - c) { }
		Vertex3 Translate(Vector2 pos) {
			a += pos;
			b += pos;
			c += pos;
			a_b = b - a;
			b_c = c - b;
			c_a = a - c;
		}
		Vertex3 Rotate(float rad) {
			float s_ = sinf(rad);
			float c_ = cosf(rad);
			a.x = a.x * +c_ + a.y * -s_;
			a.y = a.x * +s_ + a.y * +c_;
			b.x = b.x * +c_ + b.y * -s_;
			b.y = b.x * +s_ + b.y * +c_;
			c.x = c.x * +c_ + c.y * -s_;
			c.y = c.x * +s_ + c.y * +c_;
			a_b = b - a;
			b_c = c - b;
			c_a = a - c;
		}
		Vertex3 Scale(Vector2 scl) {
			a.x *= scl.x;
			a.y *= scl.y;
			b.x *= scl.x;
			b.y *= scl.y;
			c.x *= scl.x;
			c.y *= scl.y;
			a_b = b - a;
			b_c = c - b;
			c_a = a - c;
		}
	};
	struct Vertex4 {
	protected:
		friend Collider2D;
		Vector2 a_b;
		Vector2 b_c;
		Vector2 c_d;
		Vector2 d_a;


	public:
		union {
			struct {
				Vector2 a;
				Vector2 b;
				Vector2 c;
				Vector2 d;
			};
			Vector2 v[4];
		};

		Vertex4(Vector2 a, Vector2 b, Vector2 c, Vector2 d) : a(a), b(b), c(c), d(d), a_b(b - a), b_c(c - b), c_d(d - c), d_a(a - d) { }
		Vertex4 Translate(Vector2 pos) {
			a += pos;
			b += pos;
			c += pos;
			d += pos;
			a_b = b - a;
			b_c = c - b;
			c_d = d - c;
			d_a = a - d;
		}
		Vertex4 Rotate(float rad) {
			float s_ = sinf(rad);
			float c_ = cosf(rad);
			a.x = a.x * +c_ + a.y * -s_;
			a.y = a.x * +s_ + a.y * +c_;
			b.x = b.x * +c_ + b.y * -s_;
			b.y = b.x * +s_ + b.y * +c_;
			c.x = c.x * +c_ + c.y * -s_;
			c.y = c.x * +s_ + c.y * +c_;
			d.x = d.x * +c_ + d.y * -s_;
			d.y = d.x * +s_ + d.y * +c_;
			a_b = b - a;
			b_c = c - b;
			c_d = d - c;
			d_a = a - d;
		}
		Vertex4 Scale(Vector2 scl) {
			a.x *= scl.x;
			a.y *= scl.y;
			b.x *= scl.x;
			b.y *= scl.y;
			c.x *= scl.x;
			c.y *= scl.y;
			d.x *= scl.x;
			d.y *= scl.y;
			a_b = b - a;
			b_c = c - b;
			c_d = d - c;
			d_a = a - d;
		}
	};
	struct VertexN {
		Vector2 *v = nullptr;
		unsigned int num = 0;
		VertexN(Vector2 *v, unsigned int num) : v(v), num(num) { }
	};
	struct NearHit {
		Vector2 position, tilt;
	};
	struct RayHit {
		Vector2 position, tilt;
	};
	struct Ray : public Vertex2 {
		Ray(Vector2 position, Vector2 direction) : Vertex2(position, position + direction) { }
	};

	class Collider2D {
	public:
		static bool Touch(Vertex1 a, Vertex1 b, NearHit *hit = nullptr);
		static bool Touch(Vertex1 a, Vertex2 b, NearHit *hit = nullptr);
		static bool Touch(Vertex1 a, Vertex3 b, NearHit *hit = nullptr);
		static bool Touch(Vertex1 a, Vertex4 b, NearHit *hit = nullptr);
		static bool Touch(Vertex1 a, VertexN b, NearHit *hit = nullptr);
		static bool Touch(Vertex2 a, Vertex1 b, RayHit *hit = nullptr);
		static bool Touch(Vertex2 a, Vertex2 b, RayHit *hit = nullptr);
		static bool Touch(Vertex2 a, Vertex3 b, RayHit *hit = nullptr);
		static bool Touch(Vertex2 a, Vertex4 b, RayHit *hit = nullptr);
		static bool Touch(Vertex2 a, VertexN b, RayHit *hit = nullptr);
	};
}

#endif





