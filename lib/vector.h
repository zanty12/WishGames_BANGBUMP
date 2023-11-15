/*==============================================================================

    Title		: VECTOR
    Description : ベクトル
    Date		:
    Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include <math.h>

/*------------------------------------------------------------------------------
* define宣言
------------------------------------------------------------------------------*/
#define TO_POSITION2(v) v.x, v.y,
#define TO_POSITION3(v) v.x, v.y, v.z,

/*------------------------------------------------------------------------------
* extern宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
*	前方宣言
------------------------------------------------------------------------------*/
class Vector2;
class Vector3;
class Vector4;
class Vector2Int;
class Vector3Int;
class Vector4Int;

/*------------------------------------------------------------------------------
* class宣言
------------------------------------------------------------------------------*/
class Vector2 {
public:
	float x = 0.0f, y = 0.0f;

	static Vector2 Zero;
	static Vector2 One;
	static Vector2 Up;
	static Vector2 Down;
	static Vector2 Right;
	static Vector2 Left;


public:
	Vector2() {}
	Vector2(float x, float y) : x(x), y(y) {}

	Vector2 operator+ ();
	Vector2 operator- ();
	Vector2 operator+ (Vector2 v);
	Vector2 operator- (Vector2 v);
	float operator* (Vector2 v);
	friend Vector2 operator* (Vector2 v, float scaler) { return Vector2(v.x * scaler, v.y * scaler); }
	friend Vector2 operator/ (Vector2 v, float scaler) { return Vector2(v.x / scaler, v.y / scaler); }
	friend Vector2 operator* (float scaler, Vector2 v) { return Vector2(scaler * v.x, scaler * v.y); }
	friend Vector2 operator/ (float scaler, Vector2 v) { return Vector2(scaler / v.x, scaler / v.y); }

	Vector2 operator+= (Vector2 v);
	Vector2 operator-= (Vector2 v);
	friend Vector2 operator*= (Vector2 v, float scaler) { return Vector2(v.x *= scaler, v.y *= scaler); }
	friend Vector2 operator/= (Vector2 v, float scaler) { return Vector2(v.x /= scaler, v.y /= scaler); }
	friend Vector2 operator*= (float scaler, Vector2 v) { return Vector2(scaler *= v.x, scaler *= v.y); }
	friend Vector2 operator/= (float scaler, Vector2 v) { return Vector2(scaler /= v.x, scaler /= v.y); }

	bool operator== (Vector2 v);
	bool operator!= (Vector2 v);

	operator Vector3();
	operator Vector4();
	operator Vector2Int();
	operator Vector3Int();
	operator Vector4Int();



	float Distance(void);
	Vector2 Normal(void);
	Vector2 Normalize(void);

	static float Dot(Vector2 a, Vector2 b);
	static float Cross(Vector2 a, Vector2 b);
	static float Distance(Vector2 a, Vector2 b);
};
class Vector3 {
public:
	float x = 0.0f, y = 0.0f, z = 0.0f;
	
	static Vector3 Zero;
	static Vector3 One;
	static Vector3 Up;
	static Vector3 Down;
	static Vector3 Right;
	static Vector3 Left;
	static Vector3 Forward;
	static Vector3 Back;



public:
	Vector3() {}
	Vector3(float x, float y) : x(x), y(y), z(0.0f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3 operator+ ();
	Vector3 operator- ();
	Vector3 operator+ (Vector3 v);
	Vector3 operator- (Vector3 v);
	float operator* (Vector3 v);
	friend Vector3 operator* (Vector3 v, float scaler) { return Vector3(v.x * scaler, v.y * scaler, v.z * scaler); }
	friend Vector3 operator/ (Vector3 v, float scaler) { return Vector3(v.x / scaler, v.y / scaler, v.z / scaler); }
	friend Vector3 operator* (float scaler, Vector3 v) { return Vector3(scaler * v.x, scaler * v.y, scaler * v.z); }
	friend Vector3 operator/ (float scaler, Vector3 v) { return Vector3(scaler / v.x, scaler / v.y, scaler / v.z); }

	Vector3 operator+= (Vector3 v);
	Vector3 operator-= (Vector3 v);
	friend Vector3 operator*= (Vector3 v, float scaler) { return Vector3(v.x *= scaler, v.y *= scaler, v.z *= scaler); }
	friend Vector3 operator/= (Vector3 v, float scaler) { return Vector3(v.x /= scaler, v.y /= scaler, v.z /= scaler); }
	friend Vector3 operator*= (float scaler, Vector3 v) { return Vector3(scaler *= v.x, scaler *= v.y, scaler *= v.z); }
	friend Vector3 operator/= (float scaler, Vector3 v) { return Vector3(scaler /= v.x, scaler /= v.y, scaler /= v.z); }

	bool operator== (Vector3 v);
	bool operator!= (Vector3 v);

	operator Vector4();
	operator Vector3Int();
	operator Vector4Int();



	float Distance(void);
	Vector3 Normalize(void);
	Vector2 Compress(void);

	static float Dot(Vector3 a, Vector3 b);
	static Vector3 Cross(Vector3 a, Vector3 b);
	static float Distance(Vector3 a, Vector3 b);
};
class Vector4 {
public:
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;



public:
	Vector4() {}
	Vector4(float x, float y, float z) : x(x), y(y), z(z), w(0.0f) {}
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	Vector4 operator+ ();
	Vector4 operator- ();
	Vector4 operator+ (Vector4 v);
	Vector4 operator- (Vector4 v);
	float operator* (Vector4 v);
	friend Vector4 operator* (Vector4 v, float scaler) { return Vector4(v.x * scaler, v.y * scaler, v.z * scaler, v.w * scaler); }
	friend Vector4 operator/ (Vector4 v, float scaler) { return Vector4(v.x / scaler, v.y / scaler, v.z / scaler, v.w / scaler); }
	friend Vector4 operator* (float scaler, Vector4 v) { return Vector4(scaler * v.x, scaler * v.y, scaler * v.z, scaler * v.w); }
	friend Vector4 operator/ (float scaler, Vector4 v) { return Vector4(scaler / v.x, scaler / v.y, scaler / v.z, scaler / v.w); }

	Vector4 operator+= (Vector4 v);
	Vector4 operator-= (Vector4 v);
	friend Vector4 operator*= (Vector4 v, float scaler) { return Vector4(v.x *= scaler, v.y *= scaler, v.z *= scaler, v.w *= scaler); }
	friend Vector4 operator/= (Vector4 v, float scaler) { return Vector4(v.x /= scaler, v.y /= scaler, v.z /= scaler, v.w /= scaler); }
	friend Vector4 operator*= (float scaler, Vector4 v) { return Vector4(scaler *= v.x, scaler *= v.y, scaler *= v.z, scaler *= v.w); }
	friend Vector4 operator/= (float scaler, Vector4 v) { return Vector4(scaler /= v.x, scaler /= v.y, scaler /= v.z, scaler /= v.w); }

	bool operator== (Vector4 v);
	bool operator!= (Vector4 v);

	operator Vector4Int();

	Vector3 Compress(void);
};



class Vector2Int {
public:
	int x = 0.0f, y = 0.0f;



public:
	Vector2Int() {}
	Vector2Int(int x, int y) : x(x), y(y) {}

	Vector2Int operator+ (Vector2Int v);
	Vector2Int operator- (Vector2Int v);
	int operator* (Vector2Int v);
	friend Vector2Int operator* (Vector2Int v, int scaler) { return Vector2Int(v.x * scaler, v.y * scaler); }
	friend Vector2Int operator/ (Vector2Int v, int scaler) { return Vector2Int(v.x / scaler, v.y / scaler); }
	friend Vector2Int operator* (int scaler, Vector2Int v) { return Vector2Int(scaler * v.x, scaler * v.y); }
	friend Vector2Int operator/ (int scaler, Vector2Int v) { return Vector2Int(scaler / v.x, scaler / v.y); }

	Vector2Int operator+= (Vector2Int v);
	Vector2Int operator-= (Vector2Int v);
	friend Vector2Int operator*= (Vector2Int v, int scaler) { return Vector2Int(v.x *= scaler, v.y *= scaler); }
	friend Vector2Int operator/= (Vector2Int v, int scaler) { return Vector2Int(v.x /= scaler, v.y /= scaler); }
	friend Vector2Int operator*= (int scaler, Vector2Int v) { return Vector2Int(scaler *= v.x, scaler *= v.y); }
	friend Vector2Int operator/= (int scaler, Vector2Int v) { return Vector2Int(scaler /= v.x, scaler /= v.y); }

	bool operator== (Vector2Int v);
	bool operator!= (Vector2Int v);

	operator Vector2();
	operator Vector3();
	operator Vector4();
	operator Vector3Int();
	operator Vector4Int();



	int Distance(void);
	Vector2Int Normal(void);
	Vector2Int Normalize(void);

	static int Dot(Vector2Int a, Vector2Int b);
	static int Cross(Vector2Int a, Vector2Int b);
	static int Distance(Vector2Int a, Vector2Int b);
};
class Vector3Int {
public:
	int x = 0.0f, y = 0.0f, z = 0.0f;



public:
	Vector3Int() {}
	Vector3Int(int x, int y) : x(x), y(y), z(0) {}
	Vector3Int(int x, int y, int z) : x(x), y(y), z(z) {}

	Vector3Int operator+ (Vector3Int v);
	Vector3Int operator- (Vector3Int v);
	int operator* (Vector3Int v);
	friend Vector3Int operator* (Vector3Int v, int scaler) { return Vector3Int(v.x * scaler, v.y * scaler, v.z * scaler); }
	friend Vector3Int operator/ (Vector3Int v, int scaler) { return Vector3Int(v.x / scaler, v.y / scaler, v.z / scaler); }
	friend Vector3Int operator* (int scaler, Vector3Int v) { return Vector3Int(scaler * v.x, scaler * v.y, scaler * v.z); }
	friend Vector3Int operator/ (int scaler, Vector3Int v) { return Vector3Int(scaler / v.x, scaler / v.y, scaler / v.z); }

	Vector3Int operator+= (Vector3Int v);
	Vector3Int operator-= (Vector3Int v);
	friend Vector3Int operator*= (Vector3Int v, int scaler) { return Vector3Int(v.x *= scaler, v.y *= scaler, v.z *= scaler); }
	friend Vector3Int operator/= (Vector3Int v, int scaler) { return Vector3Int(v.x /= scaler, v.y /= scaler, v.z /= scaler); }
	friend Vector3Int operator*= (int scaler, Vector3Int v) { return Vector3Int(scaler *= v.x, scaler *= v.y, scaler *= v.z); }
	friend Vector3Int operator/= (int scaler, Vector3Int v) { return Vector3Int(scaler /= v.x, scaler /= v.y, scaler /= v.z); }

	bool operator== (Vector3Int v);
	bool operator!= (Vector3Int v);

	operator Vector3();
	operator Vector4();
	operator Vector4Int();



	int Distance(void);
	Vector3Int Normalize(void);
	Vector2Int Compress(void);

	static int Dot(Vector3Int a, Vector3Int b);
	static Vector3Int Cross(Vector3Int a, Vector3Int b);
	static int Distance(Vector3Int a, Vector3Int b);
};
class Vector4Int {
public:
	int x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;



public:
	Vector4Int() {}
	Vector4Int(int x, int y) : x(x), y(y), z(0), w(0) {}
	Vector4Int(int x, int y, int z) : x(x), y(y), z(z), w(0) {}
	Vector4Int(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

	Vector4Int operator+ (Vector4Int v);
	Vector4Int operator- (Vector4Int v);
	int operator* (Vector4Int v);
	friend Vector4Int operator* (Vector4Int v, int scaler) { return Vector4Int(v.x * scaler, v.y * scaler, v.z * scaler, v.w * scaler); }
	friend Vector4Int operator/ (Vector4Int v, int scaler) { return Vector4Int(v.x / scaler, v.y / scaler, v.z / scaler, v.w / scaler); }
	friend Vector4Int operator* (int scaler, Vector4Int v) { return Vector4Int(scaler * v.x, scaler * v.y, scaler * v.z, scaler * v.w); }
	friend Vector4Int operator/ (int scaler, Vector4Int v) { return Vector4Int(scaler / v.x, scaler / v.y, scaler / v.z, scaler / v.w); }

	Vector4Int operator+= (Vector4Int v);
	Vector4Int operator-= (Vector4Int v);
	friend Vector4Int operator*= (Vector4Int v, int scaler) { return Vector4Int(v.x *= scaler, v.y *= scaler, v.z *= scaler, v.w *= scaler); }
	friend Vector4Int operator/= (Vector4Int v, int scaler) { return Vector4Int(v.x /= scaler, v.y /= scaler, v.z /= scaler, v.w /= scaler); }
	friend Vector4Int operator*= (int scaler, Vector4Int v) { return Vector4Int(scaler *= v.x, scaler *= v.y, scaler *= v.z, scaler *= v.w); }
	friend Vector4Int operator/= (int scaler, Vector4Int v) { return Vector4Int(scaler /= v.x, scaler /= v.y, scaler /= v.z, scaler /= v.w); }

	bool operator== (Vector4Int v);
	bool operator!= (Vector4Int v);

	operator Vector4();

	Vector3Int Compress(void);
};

#endif