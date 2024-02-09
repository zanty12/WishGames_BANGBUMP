#include "collider2d.h"
#include "math.h"

namespace PHYSICS {
	bool Collider2D::Touch(Vertex1 a, Vertex1 b, NearHit *hit) {
		// îºåaÇÃç≈ëÂ
		float maxRadius = a.radius + b.radius;
		// å¸Ç´ÇãÅÇﬂÇÈ
		Vector2 direction = a.a - b.a;
		float distance = direction.Distance();

		if (hit) {
			Vector2 normalize = direction.Normalize();
			hit->position = a.a + direction.Normalize() * distance;
			hit->tilt = normalize.Normal();
		}

		return distance <= maxRadius;
	}
	bool Collider2D::Touch(Vertex1 a, Vertex2 b, NearHit *hit) {
		// îºåaÇÃç≈ëÂ
		float maxRadius = a.radius;
		// ê¸ï™ÇãÅÇﬂÇÈ
		Vector2 segment = b.a_b;
		Vector2 normalize = segment.Normalize();
		float distance = segment.Distance();
		// å¸Ç´ÇãÅÇﬂÇÈ
		Vector2 direction = a.a - b.a;
		float horizontal = Vector2::Dot(normalize, direction);


		// îªíË
		if (0 <= horizontal && horizontal <= distance) {
			float vertical = Vector2::Cross(normalize, direction);
			Vector2 normal = segment.Normal();

			if (hit) {
				if (0 < vertical) {
					hit->position = b.a + normalize * horizontal;
					hit->tilt = normalize;
				}
				else {
					hit->position = b.a + normalize * horizontal;
					hit->tilt = normalize * -1;
				}
			}

			return MATH::Abs(vertical) <= maxRadius;
		}
		else if (horizontal <= 0) return Touch(a, Vertex1(b.a), hit);
		else return Touch(a, Vertex1(b.b), hit);

		return false;
	}
	bool Collider2D::Touch(Vertex1 a, Vertex3 b, NearHit *hit) {
		return Touch(a, VertexN(b.v, 3), hit);
	}
	bool Collider2D::Touch(Vertex1 a, Vertex4 b, NearHit *hit) {
		return Touch(a, VertexN(b.v, 4), hit);
	}
	bool Collider2D::Touch(Vertex1 a, VertexN b, NearHit *hit) {
		// îºåaÇÃç≈ëÂ
		float maxRadius = a.radius;
		// ç≈è¨ílÇÃãLò^
		float minVertical = -1.0f;
		// è’ìÀîªíËÇÃãLò^
		bool isTouch = true;
		// ÉqÉbÉgÇÃãLò^
		NearHit resultHit;


		for (int i = 0; i < b.num; i++)
		{
			// ê¸ï™ÇãÅÇﬂÇÈ
			Vector2 start = b.v[i];
			Vector2 end = b.v[(i + 1) % b.num];
			Vector2 segment = end - start;
			Vector2 normalize = segment.Normalize();
			// å¸Ç´ÇãÅÇﬂÇÈ
			Vector2 direction = a.a - start;


			// îªíË
			float vertical = Vector2::Cross(normalize, direction);

			if (maxRadius < vertical) isTouch = false;
			if (hit == nullptr && isTouch == false) return isTouch;
			if (hit) {
				// ç≈è¨ílÇãLò^Ç∑ÇÈ
				float absVertical = MATH::Abs(vertical);
				if (absVertical < minVertical || minVertical < 0) {
					float horizontal = Vector2::Dot(normalize, direction);
					resultHit.position = start + normalize * horizontal;
					resultHit.tilt = normalize;
					minVertical = absVertical;
				}
			}
		}

		if (hit) *hit = resultHit;
		return isTouch;
	}
	bool Collider2D::Touch(Vertex2 a, Vertex1 b, RayHit *hit) {
		// îºåaÇÃç≈ëÂ
		float maxRadius = b.radius;
		// ê¸ï™ÇãÅÇﬂÇÈ
		Vector2 segment = a.a_b;
		Vector2 normalize = segment.Normalize();
		Vector2 normal = normalize.Normal();
		// å¸Ç´ÇãÅÇﬂÇÈ
		Vector2 direction = b.a - a.a;


		// îªíË
		float vertical = Vector2::Cross(normalize, direction);
		float horizontal = sqrtf(-vertical * vertical + b.radius * b.radius);
		Vector2 localPosition = (normal * -vertical) - (normalize * horizontal);
		Vector2 hitPosition = b.a + localPosition;

		if (hit) {
			hit->position = hitPosition;
			hit->tilt = localPosition.Normalize().Normal();
		}

		float hs = Vector2::Dot(normalize, hitPosition - a.a);
		float he = Vector2::Dot(normalize, hitPosition - a.b);

		if (maxRadius < MATH::Abs(vertical) || 0 < hs * he) return false;
		else return true;
	}
	bool Collider2D::Touch(Vertex2 a, Vertex2 b, RayHit *hit) {
		Vector2 as_ae = a.a_b;
		Vector2 as_bs = b.a - a.a;
		Vector2 as_be = b.b - a.a;
		Vector2 bs_be = b.a_b;
		Vector2 bs_as = a.a - b.a;
		Vector2 bs_ae = a.b - b.a;

		// åì_
		if (hit) {
			Vector2 normalize = bs_be.Normalize();
			Vector2 normal = normalize.Normal();
			float t1 = MATH::Abs(Vector2::Dot(normal, bs_as));
			float t2 = MATH::Abs(Vector2::Dot(normal, bs_ae));
			hit->position = (a.a * t2 + a.b * t1) / (t1 + t2);
			hit->tilt = normalize;
		}

		// åç∑îªíË
		return Vector2::Cross(as_ae, as_bs) * Vector2::Cross(as_ae, as_be) <= 0 &&
			Vector2::Cross(bs_be, bs_as) * Vector2::Cross(bs_be, bs_ae) <= 0;
	}
	bool Collider2D::Touch(Vertex2 a, Vertex3 b, RayHit *hit) {
		return Touch(a, VertexN(b.v, 3), hit);
	}
	bool Collider2D::Touch(Vertex2 a, Vertex4 b, RayHit *hit) {
		return Touch(a, VertexN(b.v, 4), hit);
	}
	bool Collider2D::Touch(Vertex2 a, VertexN b, RayHit *hit) {
		// ç≈è¨ílÇÃãLò^
		float minDistance = -1.0f;
		// è’ìÀîªíËÇÃãLò^
		bool isTouch = true;
		// ÉqÉbÉgÇÃãLò^
		RayHit resultHit;


		for (int i = 0; i < b.num; i++)
		{
			RayHit _hit;

			// ê¸ï™ÇãÅÇﬂÇÈ
			Vector2 start = b.v[i];
			Vector2 end = b.v[(i + 1) % b.num];
			Touch(a, Vertex2(start, end), hit ? &_hit : nullptr);

			if (hit) {
				Vector2 direction = end - a.a;
				float distance = direction.Distance();
				if (distance < minDistance || minDistance < 0) {
					resultHit = _hit;
				}
			}
		}

		if (hit) *hit = resultHit;
		return isTouch;
	}
}