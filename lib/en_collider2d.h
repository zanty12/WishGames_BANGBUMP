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
* include�錾
------------------------------------------------------------------------------*/
#include "vector.h"
#include "math.h"

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
        /// �d�S���擾
        /// </summary>
        Vector2 GetCenterOfGravity(void);





        /// <summary>
        /// ��_���擾����
        /// </summary>
        /// <param name="startA">�n�_A</param>
        /// <param name="endA">�I�_A</param>
        /// <param name="startB">�n�_B</param>
        /// <param name="endB">�I�_B</param>
        /// <param name="intersection">��_</param>
        /// <returns></returns>
        static bool IsIntersectionSegmentAndSegment(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB, Vector2 *intersection);

        /// <summary>
        /// �����Ɠ_�̔���
        /// </summary>
        /// <param name="ray">����</param>
        /// <param name="dot">�_</param>
        /// <param name="hit">��_</param>
        /// <returns></returns>
        static bool IsRayAndDot(Ray ray, DOT dot, RayHit *hit);

        /// <summary>
        /// �����Ɛ����̔���
        /// </summary>
        /// <param name="ray">����</param>
        /// <param name="segment">����</param>
        /// <param name="hit">��_</param>
        /// <returns></returns>
        static bool IsRayAndSegment(Ray ray, SEGMENT segment, RayHit *hit);

        /// <summary>
        /// �����ƃ|���S���̔���
        /// </summary>
        /// <param name="ray">����</param>
        /// <param name="triangle">�|���S��</param>
        /// <param name="hit">��_</param>
        /// <returns></returns>
        static bool IsRayAndTriangle(Ray ray, TRIANGLE triangle, RayHit *hit);

        /// <summary>
        /// �_����̍ŒZ�������擾
        /// </summary>
        /// <param name="point">�v���n�_</param>
        /// <param name="dot">�_</param>
        /// <param name="hit">�ŒZ����</param>
        /// <returns></returns>
        static bool IsNearAndDot(Vector2 point, DOT dot, NearHit *hit);

        /// <summary>
        /// �_����̍ŒZ�������擾
        /// </summary>
        /// <param name="point">�v���n�_</param>
        /// <param name="seg">����</param>
        /// <param name="hit">�ŒZ����</param>
        /// <returns></returns>
        static bool IsNearAndSegment(Vector2 point, SEGMENT segment, NearHit *hit);

        /// <summary>
        /// �_����̍ŒZ�������擾
        /// </summary>
        /// <param name="point">�v���n�_</param>
        /// <param name="triangle">�|���S��</param>
        /// <param name="hit">�ŒZ����</param>
        /// <returns></returns>
        static bool IsNearAndTriangle(Vector2 point, TRIANGLE triangle, NearHit *hit);

        /// <summary>
        /// �_�Ɠ_�̔���
        /// </summary>
        /// <param name="a">�_A</param>
        /// <param name="b">�_B</param>
        /// <returns></returns>
        static bool IsDotAndDot(DOT a, DOT b);

        /// <summary>
        /// �����Ɠ_�̔���
        /// </summary>
        /// <param name="a">����</param>
        /// <param name="b">�_</param>
        /// <returns></returns>
        static bool IsSegmentAndDot(SEGMENT a, DOT b);

        /// <summary>
        /// �����Ɛ����̔���
        /// </summary>
        /// <param name="a">����</param>
        /// <param name="b">�_</param>
        /// <returns></returns>
        static bool IsSegmentAndSegment(SEGMENT a, SEGMENT b);

        /// <summary>
        /// �|���S���Ɠ_�̔���
        /// </summary>
        /// <param name="a">�|���S��</param>
        /// <param name="b">�_</param>
        /// <returns></returns>
        static bool IsTriangleAndDot(TRIANGLE a, DOT b);

        /// <summary>
        /// �|���S���Ɛ����̔���
        /// </summary>
        /// <param name="a">�|���S��</param>
        /// <param name="b">����</param>
        /// <returns></returns>
        static bool IsTriangleAndSegment(TRIANGLE a, SEGMENT b);

        /// <summary>
        /// �|���S���ƃ|���S���̔���
        /// </summary>
        /// <param name="a">�|���S��</param>
        /// <param name="b">�|���S��</param>
        /// <returns></returns>
        static bool IsTriangleAndTriangle(TRIANGLE a, TRIANGLE b);

        /// <summary>
        /// �R���C�_�[�ƃR���C�_�[�̊ȗ�����
        /// </summary>
        /// <param name="a">�R���C�_�[</param>
        /// <param name="b">�R���C�_�[</param>
        /// <returns></returns>
        static bool IsRoughlyTouch(Collider2D a, Collider2D b);

        /// <summary>
        /// �R���C�_�[�ƃR���C�_�[�̔���
        /// </summary>
        /// <param name="a">�R���C�_�[</param>
        /// <param name="b">�R���C�_�[</param>
        /// <returns></returns>
        static bool IsTouch(Collider2D *a, Collider2D *b);

        /// <summary>
        /// �����ƃR���C�_�[�̔���
        /// </summary>
        /// <param name="ray">����</param>
        /// <param name="a">�R���C�_�[</param>
        /// <param name="hit">�q�b�g</param>
        /// <returns></returns>
        static bool IsTouch(Ray ray, Collider2D *collider, RayHit *hit);

        /// <summary>
        /// �_�ƃR���C�_�[�̍ŒZ��������
        /// </summary>
        /// <param name="point">�_</param>
        /// <param name="a">�R���C�_�[</param>
        /// <param name="hit">�q�b�g</param>
        /// <returns></returns>
        static bool IsTouch(Vector2 point, Collider2D *collider, NearHit *hit);
    };
}

#endif
