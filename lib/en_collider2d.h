/*==============================================================================

    Title		: Collider
    Description :
    Date		:
    Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _EN_COLLIDER2D_H_
#define _EN_COLLIDER2D_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include "vector.h"
#include "math.h"

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
* enum宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class宣言
------------------------------------------------------------------------------*/
namespace ENGINE {
    class Collider2D {
    public:
        enum ColliderType {
            CIRCLE,
            BOX
        };

        class Ray {
        public:
            Vector2 position, direction;

            Ray() { }
            Ray(Vector2 position, Vector2 direction) : position(position), direction(direction) { }
        };
        class RayHit {
        public:
            Vector2 position, tilt;
        };
        class NearHit {
        public:
            Vector2 position, tilt;
        };

        class DOT {
        public:
            Vector2 point;
            float radius = 0.0f;
        };
        class SEGMENT {
        public:
            SEGMENT() { }
            SEGMENT(Vector2 start, Vector2 end, float radius) : start(start), end(end), radius(radius) { }

            Vector2 start, end;
            float radius = 0.0f;
        };
        class TRIANGLE {
        public:
            TRIANGLE() { }
            TRIANGLE(Vector2 a, Vector2 b, Vector2 c, float radius) : a(a), b(b), c(c), radius(radius) { }

            Vector2 a, b, c;
            float radius = 0.0f;
        };

    public:
        bool isFill = false;
        float radius = 100.0f;
        unsigned int count = 1;
        Vector2 *vertex = nullptr;


    public:
        /// <summary>
        /// 重心を取得
        /// </summary>
        Vector2 GetCenterOfGravity(void);





        /// <summary>
        /// 交点を取得する
        /// </summary>
        /// <param name="startA">始点A</param>
        /// <param name="endA">終点A</param>
        /// <param name="startB">始点B</param>
        /// <param name="endB">終点B</param>
        /// <param name="intersection">交点</param>
        /// <returns></returns>
        static bool IsIntersectionSegmentAndSegment(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB, Vector2 *intersection);

        /// <summary>
        /// 光線と点の判定
        /// </summary>
        /// <param name="ray">光線</param>
        /// <param name="dot">点</param>
        /// <param name="hit">交点</param>
        /// <returns></returns>
        static bool IsRayAndDot(Ray ray, DOT dot, RayHit *hit);

        /// <summary>
        /// 光線と線分の判定
        /// </summary>
        /// <param name="ray">光線</param>
        /// <param name="segment">線分</param>
        /// <param name="hit">交点</param>
        /// <returns></returns>
        static bool IsRayAndSegment(Ray ray, SEGMENT segment, RayHit *hit);

        /// <summary>
        /// 光線とポリゴンの判定
        /// </summary>
        /// <param name="ray">光線</param>
        /// <param name="triangle">ポリゴン</param>
        /// <param name="hit">交点</param>
        /// <returns></returns>
        static bool IsRayAndTriangle(Ray ray, TRIANGLE triangle, RayHit *hit);

        /// <summary>
        /// 点からの最短距離を取得
        /// </summary>
        /// <param name="point">計測地点</param>
        /// <param name="dot">点</param>
        /// <param name="hit">最短距離</param>
        /// <returns></returns>
        static bool IsNearAndDot(Vector2 point, DOT dot, NearHit *hit);

        /// <summary>
        /// 点からの最短距離を取得
        /// </summary>
        /// <param name="point">計測地点</param>
        /// <param name="seg">線分</param>
        /// <param name="hit">最短距離</param>
        /// <returns></returns>
        static bool IsNearAndSegment(Vector2 point, SEGMENT segment, NearHit *hit);

        /// <summary>
        /// 点からの最短距離を取得
        /// </summary>
        /// <param name="point">計測地点</param>
        /// <param name="triangle">ポリゴン</param>
        /// <param name="hit">最短距離</param>
        /// <returns></returns>
        static bool IsNearAndTriangle(Vector2 point, TRIANGLE triangle, NearHit *hit);

        /// <summary>
        /// 点と点の判定
        /// </summary>
        /// <param name="a">点A</param>
        /// <param name="b">点B</param>
        /// <returns></returns>
        static bool IsDotAndDot(DOT a, DOT b);

        /// <summary>
        /// 線分と点の判定
        /// </summary>
        /// <param name="a">線分</param>
        /// <param name="b">点</param>
        /// <returns></returns>
        static bool IsSegmentAndDot(SEGMENT a, DOT b);

        /// <summary>
        /// 線分と線分の判定
        /// </summary>
        /// <param name="a">線分</param>
        /// <param name="b">点</param>
        /// <returns></returns>
        static bool IsSegmentAndSegment(SEGMENT a, SEGMENT b);

        /// <summary>
        /// ポリゴンと点の判定
        /// </summary>
        /// <param name="a">ポリゴン</param>
        /// <param name="b">点</param>
        /// <returns></returns>
        static bool IsTriangleAndDot(TRIANGLE a, DOT b);

        /// <summary>
        /// ポリゴンと線分の判定
        /// </summary>
        /// <param name="a">ポリゴン</param>
        /// <param name="b">線分</param>
        /// <returns></returns>
        static bool IsTriangleAndSegment(TRIANGLE a, SEGMENT b);

        /// <summary>
        /// ポリゴンとポリゴンの判定
        /// </summary>
        /// <param name="a">ポリゴン</param>
        /// <param name="b">ポリゴン</param>
        /// <returns></returns>
        static bool IsTriangleAndTriangle(TRIANGLE a, TRIANGLE b);

        /// <summary>
        /// コライダーとコライダーの簡略判定
        /// </summary>
        /// <param name="a">コライダー</param>
        /// <param name="b">コライダー</param>
        /// <returns></returns>
        static bool IsRoughlyTouch(Collider2D a, Collider2D b);

        /// <summary>
        /// コライダーとコライダーの判定
        /// </summary>
        /// <param name="a">コライダー</param>
        /// <param name="b">コライダー</param>
        /// <returns></returns>
        static bool IsTouch(Collider2D *a, Collider2D *b);

        /// <summary>
        /// 光線とコライダーの判定
        /// </summary>
        /// <param name="ray">光線</param>
        /// <param name="a">コライダー</param>
        /// <param name="hit">ヒット</param>
        /// <returns></returns>
        static bool IsTouch(Ray ray, Collider2D *collider, RayHit *hit);

        /// <summary>
        /// 点とコライダーの最短距離判定
        /// </summary>
        /// <param name="point">点</param>
        /// <param name="a">コライダー</param>
        /// <param name="hit">ヒット</param>
        /// <returns></returns>
        static bool IsTouch(Vector2 point, Collider2D *collider, NearHit *hit);
    };
}

#endif
