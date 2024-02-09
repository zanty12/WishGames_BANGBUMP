#include "en_collider2d.h"


namespace ENGINE {
    Vector2 Collider2D::GetCenterOfGravity(void) {
        Vector2 centerOfGravity;
        for (int i = 0; i < count; i++) {
            centerOfGravity += vertex[i];
        }
        return centerOfGravity / count;
    }
    bool Collider2D::IsIntersectionSegmentAndSegment(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB, Vector2 *intersection) {
        Vector2 as_ae = endA - startA;
        Vector2 as_bs = startB - startA;
        Vector2 as_be = endB - startA;
        Vector2 bs_be = endB - startB;
        Vector2 bs_as = startA - startB;
        Vector2 bs_ae = endA - startB;

        if (intersection) {
            Vector2 nor = bs_be.Normal();
            float t1 = MATH::Abs(Vector2::Dot(nor, bs_as));
            float t2 = MATH::Abs(Vector2::Dot(nor, bs_ae));
            *intersection = (startA * t2 + endA * t1) / (t1 + t2);
        }

        // 交差判定
        return Vector2::Cross(as_ae, as_bs) * Vector2::Cross(as_ae, as_be) <= 0 &&
            Vector2::Cross(bs_be, bs_as) * Vector2::Cross(bs_be, bs_ae) <= 0;
    }





    bool Collider2D::IsRayAndDot(Ray ray, DOT dot, RayHit *hit) {
        Vector2 direction = dot.point - ray.position;

        Vector2 normalize = ray.direction.Normalize();
        Vector2 normal = ray.direction.Normal();

        float vertical = Vector2::Cross(normalize, direction);
        float horizontal = sqrtf(-vertical * vertical + dot.radius * dot.radius);
        Vector2 localPosition = (normal * -vertical) - (normalize * horizontal);
        Vector2 hitPosition = dot.point + localPosition;

        float hs = Vector2::Dot(normalize, hitPosition - ray.position);
        float he = Vector2::Dot(normalize, hitPosition - (ray.position + ray.direction));

        // 触れていないなら終了
        if (dot.radius < MATH::Abs(vertical) || 0 < hs * he) return false;

        if (hit) {
            hit->position = hitPosition;
            hit->tilt = localPosition.Normal() * -1;
        }
        return true;
    }

    bool Collider2D::IsRayAndSegment(Ray ray, SEGMENT segment, RayHit *hit) {
        Vector2 direction = segment.end - segment.start;
        Vector2 normalize = direction.Normalize();
        Vector2 normal = direction.Normal();
        Vector2 rayStart = ray.position;
        Vector2 rayEnd = ray.position + ray.direction;

        RayHit resultHit;
        bool isTouch = false;
        float minDistance = -1;



        // 線分
        if (segment.radius == 0.0f) {
            isTouch = IsIntersectionSegmentAndSegment(
                rayStart,
                rayEnd,
                segment.start,
                segment.end,
                &resultHit.position
            );
            // hitの更新
            if (isTouch && hit) {
                resultHit.tilt = normalize;
                minDistance = (resultHit.position - ray.position).Distance();
                *hit = resultHit;
            }
            return isTouch;
        }
        //カプセル
        else {
            // 線分の判定
            isTouch = IsIntersectionSegmentAndSegment(
                rayStart,
                rayEnd,
                segment.start + normal * segment.radius,
                segment.end + normal * segment.radius,
                &resultHit.position
            );
            // hitの更新
            if (isTouch && hit) {
                resultHit.tilt = normalize;
                minDistance = (resultHit.position - ray.position).Distance();
                *hit = resultHit;
            }

            isTouch = IsIntersectionSegmentAndSegment(
                rayStart,
                rayEnd,
                segment.start - normal * segment.radius,
                segment.end - normal * segment.radius,
                &resultHit.position
            );
            // hitの更新
            if (isTouch && hit) {
                float distance = (resultHit.position - ray.position).Distance();
                if (distance < minDistance || minDistance < 0) {
                    resultHit.tilt = normalize * -1;
                    minDistance = distance;
                    *hit = resultHit;
                }
            }






            // 円の判定
            isTouch = IsRayAndDot(ray, { segment.start, segment.radius }, &resultHit);
            // hitの更新
            if (isTouch && hit) {
                float distance = (resultHit.position - ray.position).Distance();
                if (distance < minDistance || minDistance < 0) {
                    minDistance = distance;
                    *hit = resultHit;
                }
            }

            isTouch = IsRayAndDot(ray, { segment.end, segment.radius }, &resultHit);
            // hitの更新
            if (isTouch && hit) {
                float distance = (resultHit.position - ray.position).Distance();
                if (distance < minDistance || minDistance < 0) {
                    minDistance = distance;
                    *hit = resultHit;
                }
            }

            // -1ではないなら触れている
            return 0 <= minDistance;
        }
    }

    bool Collider2D::IsRayAndTriangle(Ray ray, TRIANGLE triangle, RayHit *hit) {
        if (IsTriangleAndSegment(triangle, { ray.position, ray.position + ray.direction, 0 })) {
            if (!hit) return true;

            RayHit a_b, b_c, c_a;
            bool isTouch1, isTouch2, isTouch3;
            float minDistance = -1;

            isTouch1 = IsRayAndSegment(ray, { triangle.a, triangle.b, triangle.radius }, &a_b);
            isTouch2 = IsRayAndSegment(ray, { triangle.b, triangle.c, triangle.radius }, &b_c);
            isTouch3 = IsRayAndSegment(ray, { triangle.c, triangle.a, triangle.radius }, &c_a);

            if (isTouch1) {
                minDistance = Vector2::Distance(a_b.position, ray.position);
                *hit = a_b;
            }
            if (isTouch2) {
                float distance = Vector2::Distance(b_c.position, ray.position);
                if (distance < minDistance || minDistance <= 0) minDistance = distance, *hit = b_c;
            }
            if (isTouch3) {
                float distance = Vector2::Distance(c_a.position, ray.position);
                if (distance < minDistance || minDistance <= 0) minDistance = distance, *hit = c_a;
            }

            return true;
        }

        return false;
    }

    bool Collider2D::IsNearAndDot(Vector2 point, DOT dot, NearHit *hit) {
        Vector2 direction = point - dot.point;
        float distance = direction.Distance();

        if (hit) {
            hit->position = dot.point + direction.Normalize() * dot.radius;
            hit->tilt = direction.Normal();
        }

        return distance <= dot.radius;
    }

    bool Collider2D::IsNearAndSegment(Vector2 point, SEGMENT seg, NearHit *hit) {
        // 線分の距離を求める
        Vector2 segment = seg.end - seg.start;
        float distance = segment.Distance();
        // 線分の正規化
        Vector2 normalize = segment.Normalize();
        // 円の半径の内積を求める
        Vector2 direction = point - seg.start;
        float horizontal = Vector2::Dot(normalize, direction);
        // 半径の最大
        float maxDistance = seg.radius;


        if (0 <= horizontal && horizontal <= distance) {
            float vertical = Vector2::Cross(normalize, direction);
            Vector2 normal = segment.Normal();

            if (hit) {
                if (0 < vertical) {
                    hit->position = seg.start + normalize * horizontal + normal * seg.radius;
                    hit->tilt = normalize;
                }
                else {
                    hit->position = seg.start + normalize * horizontal + normal * -seg.radius;
                    hit->tilt = normalize * -1;
                }
            }

            return MATH::Abs(vertical) <= maxDistance;
        }
        else if (horizontal <= 0) return IsNearAndDot(point, { seg.start, seg.radius }, hit);
        else return IsNearAndDot(point, { seg.end, seg.radius }, hit);

        return false;
    }

    bool Collider2D::IsNearAndTriangle(Vector2 point, TRIANGLE t, NearHit *hit) {

        float maxRadius = t.radius;
        bool isTouch = true;
        float minVertical = -1.0f;


        Vector2 aDirection = point - t.a;
        Vector2 bDirection = point - t.b;
        Vector2 cDirection = point - t.c;

        Vector2 a_bDirection = t.b - t.a;
        Vector2 b_cDirection = t.c - t.b;
        Vector2 c_aDirection = t.a - t.c;
        Vector2 a_bNormalize = a_bDirection.Normalize();
        Vector2 b_cNormalize = b_cDirection.Normalize();
        Vector2 c_aNormalize = c_aDirection.Normalize();

        float a_bDistance = a_bDirection.Distance();
        float b_cDistance = b_cDirection.Distance();
        float c_aDistance = c_aDirection.Distance();
        float a_bHorizontal = Vector2::Dot(a_bNormalize, aDirection);
        float b_cHorizontal = Vector2::Dot(b_cNormalize, bDirection);
        float c_aHorizontal = Vector2::Dot(c_aNormalize, cDirection);
        float a_bVertical = Vector2::Cross(a_bNormalize, aDirection);
        float b_cVertical = Vector2::Cross(b_cNormalize, bDirection);
        float c_aVertical = Vector2::Cross(c_aNormalize, cDirection);
        bool inTriangle = a_bVertical <= 0.0f && b_cVertical <= 0.0f && c_aVertical <= 0.0f;


        if (0.0f < a_bHorizontal && a_bHorizontal < a_bDistance) {
            // hitの計算
            minVertical = MATH::Abs(a_bVertical);
            hit->position = t.a + a_bNormalize * a_bHorizontal + a_bNormalize.Normal() * maxRadius;
            hit->tilt = a_bNormalize;

            // 衝突判定
            if (maxRadius < a_bVertical) isTouch = false;
        }
        if (0.0f < b_cHorizontal && b_cHorizontal < b_cDistance) {
            // hitの計算
            float absVertical = MATH::Abs(b_cVertical);
            if (absVertical < minVertical || minVertical < 0.0f) {
                minVertical = MATH::Abs(absVertical);
                hit->position = t.b + b_cNormalize * b_cHorizontal + b_cNormalize.Normal() * maxRadius;
                hit->tilt = b_cNormalize;
            }

            // 衝突判定
            if (maxRadius < b_cVertical) isTouch = false;
        }
        if (0.0f < c_aHorizontal && c_aHorizontal < c_aDistance) {
            // hitの計算
            float absVertical = MATH::Abs(c_aVertical);
            if (absVertical < minVertical || minVertical < 0.0f) {
                minVertical = MATH::Abs(absVertical);
                hit->position = t.c + c_aNormalize * c_aHorizontal + c_aNormalize.Normal() * maxRadius;
                hit->tilt = c_aNormalize;
            }

            // 衝突判定
            if (maxRadius < c_aVertical) isTouch = false;
        }

        if (a_bHorizontal <= 0.0f && c_aDistance <= c_aHorizontal) {
            if (hit) isTouch = IsNearAndDot(point, { t.a , maxRadius }, hit);
        }
        else if (b_cHorizontal <= 0.0f && a_bDistance <= a_bHorizontal) {
            if (hit) isTouch = IsNearAndDot(point, { t.b , maxRadius }, hit);
        }
        else if (c_aHorizontal <= 0.0f && b_cDistance <= b_cHorizontal) {
            if (hit) isTouch = IsNearAndDot(point, { t.c , maxRadius }, hit);
        }

        return isTouch || inTriangle;
    }



    bool Collider2D::IsDotAndDot(DOT a, DOT b) {
        float maxRadius = a.radius + b.radius;
        float distance = Vector2::Distance(a.point, b.point);
        return distance <= maxRadius;
    }

    bool Collider2D::IsSegmentAndDot(SEGMENT a, DOT b) {
        // 線分の距離を求める
        Vector2 segment = a.end - a.start;
        float distance = segment.Distance();
        // 線分の正規化
        Vector2 normalize = segment.Normalize();
        // 円の半径の内積を求める
        Vector2 direction = b.point - a.start;
        float horizontal = Vector2::Dot(normalize, direction);
        // 半径の最大
        float maxDistance = a.radius + b.radius;


        if (0 < horizontal && horizontal < distance) {
            float vertical = Vector2::Cross(normalize, direction);
            return MATH::Abs(vertical) <= maxDistance;
        }
        else if (horizontal <= 0) return IsDotAndDot({ a.start, a.radius }, b);
        else return IsDotAndDot({ a.end, a.radius }, b);
    }

    bool Collider2D::IsSegmentAndSegment(SEGMENT a, SEGMENT b) {
        if (a.start == a.end) return IsSegmentAndDot(b, { a.start, a.radius });
        if (b.start == b.end) return IsSegmentAndDot(a, { b.start, b.radius });

        Vector2 as_ae = a.end - a.start;
        Vector2 as_bs = b.start - a.start;
        Vector2 as_be = b.end - a.start;
        Vector2 bs_be = b.end - b.start;
        Vector2 bs_as = a.start - b.start;
        Vector2 bs_ae = a.end - b.start;

        // 交差判定
        if (Vector2::Cross(as_ae, as_bs) * Vector2::Cross(as_ae, as_be) <= 0 &&
            Vector2::Cross(bs_be, bs_as) * Vector2::Cross(bs_be, bs_ae) <= 0) return true;



        // 円とカプセルの判定
        if (IsSegmentAndDot(a, { b.start, b.radius })) return true;
        if (IsSegmentAndDot(a, { b.end, b.radius })) return true;
        if (IsSegmentAndDot(b, { a.start, a.radius })) return true;
        if (IsSegmentAndDot(b, { a.end, a.radius })) return true;


        return false;
    }

    bool Collider2D::IsTriangleAndDot(TRIANGLE a, DOT b) {

        float maxRadius = a.radius + b.radius;
        bool isTouch = true;


        Vector2 aDirection = b.point - a.a;
        Vector2 bDirection = b.point - a.b;
        Vector2 cDirection = b.point - a.c;

        Vector2 a_bDirection = a.b - a.a;
        Vector2 b_cDirection = a.c - a.b;
        Vector2 c_aDirection = a.a - a.c;
        Vector2 a_bNormalize = a_bDirection.Normalize();
        Vector2 b_cNormalize = b_cDirection.Normalize();
        Vector2 c_aNormalize = c_aDirection.Normalize();

        float a_bDistance = a_bDirection.Distance();
        float b_cDistance = b_cDirection.Distance();
        float c_aDistance = c_aDirection.Distance();
        float a_bHorizontal = Vector2::Dot(a_bNormalize, aDirection);
        float b_cHorizontal = Vector2::Dot(b_cNormalize, bDirection);
        float c_aHorizontal = Vector2::Dot(c_aNormalize, cDirection);
        float a_bVertical = Vector2::Cross(a_bNormalize, aDirection);
        float b_cVertical = Vector2::Cross(b_cNormalize, bDirection);
        float c_aVertical = Vector2::Cross(c_aNormalize, cDirection);
        bool inTriangle = a_bVertical <= 0.0f && b_cVertical <= 0.0f && c_aVertical <= 0.0f;

        if (0.0f < a_bHorizontal && a_bHorizontal < a_bDistance) {
            // 衝突判定
            if (maxRadius < a_bVertical) isTouch = false;
        }
        if (0.0f < b_cHorizontal && b_cHorizontal < b_cDistance) {
            // 衝突判定
            if (maxRadius < b_cVertical) isTouch = false;
        }
        if (0.0f < c_aHorizontal && c_aHorizontal < c_aDistance) {
            // 衝突判定
            if (maxRadius < c_aVertical) isTouch = false;
        }

        if (a_bHorizontal <= 0.0f && c_aDistance <= c_aHorizontal) {
            isTouch = IsDotAndDot(b, { a.a , a.radius });
        }
        else if (b_cHorizontal <= 0.0f && a_bDistance <= a_bHorizontal) {
            isTouch = IsDotAndDot(b, { a.b , a.radius });
        }
        else if (c_aHorizontal <= 0.0f && b_cDistance <= b_cHorizontal) {
            isTouch = IsDotAndDot(b, { a.c , a.radius });
        }

        return isTouch || inTriangle;
    }

    bool Collider2D::IsTriangleAndSegment(TRIANGLE a, SEGMENT b) {
        if (IsSegmentAndSegment({ a.a, a.b, a.radius }, b)) return true;
        else if (IsSegmentAndSegment({ a.b, a.c, a.radius }, b)) return true;
        else if (IsSegmentAndSegment({ a.c, a.a, a.radius }, b)) return true;

        return false;
    }

    bool Collider2D::IsTriangleAndTriangle(TRIANGLE a, TRIANGLE b) {
        if (IsTriangleAndDot(a, { b.a, b.radius })) return true;
        else if (IsTriangleAndDot(a, { b.b, b.radius })) return true;
        else if (IsTriangleAndDot(a, { b.c, b.radius })) return true;

        return false;
    }



    bool Collider2D::IsRoughlyTouch(Collider2D a, Collider2D b) {
        Vector2 aMin, aMax;
        Vector2 bMin, bMax;
        float maxRadius = a.radius + b.radius;

        for (int i = 0; i < a.count; i++) {
            if (a.vertex[i].x < aMin.x) aMin.x = a.vertex[i].x;
            else if (aMax.x < a.vertex[i].x) aMax.x = a.vertex[i].x;
            if (a.vertex[i].y < aMin.y) aMin.y = a.vertex[i].y;
            else if (aMax.y < a.vertex[i].y) aMax.y = a.vertex[i].y;
        }
        for (int i = 0; i < b.count; i++) {
            if (b.vertex[i].x < bMin.x) bMin.x = b.vertex[i].x;
            else if (bMax.x < b.vertex[i].x) bMax.x = b.vertex[i].x;
            if (b.vertex[i].y < bMin.y) bMin.y = b.vertex[i].y;
            else if (bMax.y < b.vertex[i].y) bMax.y = b.vertex[i].y;
        }

        aMin.x -= maxRadius;
        aMin.y -= maxRadius;
        aMax.x += maxRadius;
        aMax.y += maxRadius;
        bMin.x -= maxRadius;
        bMin.y -= maxRadius;
        bMax.x += maxRadius;
        bMax.y += maxRadius;

        if (bMax.x < aMin.x || aMax.x < bMin.x ||
            bMax.y < aMin.y || aMax.y < bMin.y) {
            return false;
        }
        return true;
    }

    bool Collider2D::IsTouch(Collider2D *a, Collider2D *b) {
        if (a->count == 0 || b->count == 0) return false;

        // 面と面
        if (a->isFill && b->isFill) {
            if (a->count % 3 != 0 || b->count % 3 != 0) return false;
            for (int i = 0; i < a->count; i += 3) {
                TRIANGLE ta = { a->vertex[i], a->vertex[i + 1], a->vertex[i + 2], a->radius };
                for (int j = 0; j < b->count; j += 3) {
                    TRIANGLE tb = { b->vertex[j], b->vertex[j + 1], b->vertex[j + 2], b->radius };
                    if (IsTriangleAndTriangle(ta, tb)) return true;
                }
            }
        }
        // 線と線
        else if (!a->isFill && !b->isFill) {
            // 点と線
            for (int i = 0; i < a->count; i++) {
                SEGMENT sa = { a->vertex[i], a->vertex[(i + 1) % a->count], a->radius };
                for (int j = 0; j < b->count; j++) {
                    SEGMENT sb = { b->vertex[j], b->vertex[(j + 1) % b->count], b->radius };
                    if (IsSegmentAndSegment(sa, sb)) return true;
                }
            }
        }
        // 線と面
        else {
            SEGMENT s;
            TRIANGLE t;
            int tCount, sCount;
            if (a->isFill) tCount = a->count, sCount = b->count;
            else tCount = b->count, sCount = a->count;

            for (int i = 0; i < tCount; i += 3) {
                // 面の指定
                if (a->isFill) t = { a->vertex[i], a->vertex[i + 1], a->vertex[i + 2], a->radius };
                else t = { b->vertex[i], b->vertex[i + 1], b->vertex[i + 2], b->radius };

                for (int j = 0; j < sCount; j++) {
                    // 線の指定
                    if (a->isFill) s = SEGMENT(b->vertex[j], b->vertex[(j + 1) % b->count], b->radius);
                    else s = SEGMENT(a->vertex[j], a->vertex[(j + 1) % a->count], a->radius);

                    // 判定
                    if (IsTriangleAndSegment(t, s)) return true;
                }
            }
        }

        return false;
    }

    bool Collider2D::IsTouch(Ray ray, Collider2D *a, RayHit *hit) {
        if (a->count == 0) return false;

        // 点
        if (a->count == 1) {
            DOT da = { a->vertex[0], a->radius };
            if (IsRayAndDot(ray, da, hit)) return true;
        }
        // 面
        else if (a->isFill) {
            if (a->count % 3 != 0) return false;
            for (int i = 0; i < a->count; i += 3) {
                TRIANGLE ta = { a->vertex[i], a->vertex[i + 1], a->vertex[i + 2], a->radius };
                if (IsRayAndTriangle(ray, ta, hit)) return true;
            }
        }
        // 線
        else if (!a->isFill) {
            // 点と線
            for (int i = 0; i < a->count; i++) {
                SEGMENT sa = { a->vertex[i], a->vertex[(i + 1) % a->count], a->radius };
                if (IsRayAndSegment(ray, sa, hit)) return true;
            }
        }

        return false;
    }

    bool Collider2D::IsTouch(Vector2 point, Collider2D *a, NearHit *hit) {
        if (a->count == 0) return false;

        // 点
        if (a->count == 1) {
            DOT da = { a->vertex[0], a->radius };
            if (IsNearAndDot(point, da, hit)) return true;
        }
        // 面
        else if (a->isFill) {
            if (a->count % 3 != 0) return false;
            for (int i = 0; i < a->count; i += 3) {
                TRIANGLE ta = { a->vertex[i], a->vertex[i + 1], a->vertex[i + 2], a->radius };
                if (IsNearAndTriangle(point, ta, hit)) return true;
            }
        }
        // 線
        else if (!a->isFill) {
            // 点と線
            for (int i = 0; i < a->count; i++) {
                SEGMENT sa = { a->vertex[i], a->vertex[(i + 1) % a->count], a->radius };
                if (IsNearAndSegment(point, sa, hit)) return true;
            }
        }

        return false;
    }
}
