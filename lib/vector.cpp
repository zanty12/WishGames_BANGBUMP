#include "vector.h"

Vector2 Vector2::Zero		= Vector2(0.f, 0.f);
Vector2 Vector2::One		= Vector2(1.f, 1.f);
Vector2 Vector2::Up			= Vector2(0.f, 1.f);
Vector2 Vector2::Down		= Vector2(0.f, -1.f);
Vector2 Vector2::Right		= Vector2(1.f, 0.f);
Vector2 Vector2::Left		= Vector2(-1.f, 0.f);

Vector3 Vector3::Zero		= Vector3(0.f, 0.f, 0.f);
Vector3 Vector3::One		= Vector3(1.f, 1.f, 1.f);
Vector3 Vector3::Up			= Vector3(0.f, 1.f, 0.f);
Vector3 Vector3::Down		= Vector3(0.f, -1.f, 0.f);
Vector3 Vector3::Right		= Vector3(1.f, 0.f, 0.f);
Vector3 Vector3::Left		= Vector3(-1.f, 0.f, 0.f);
Vector3 Vector3::Forward	= Vector3(0.f, 0.f, 1.f);
Vector3 Vector3::Back		= Vector3(0.f, 0.f, -1.f);




Vector2 Vector2::operator+ () { return Vector2(+x, +y); }
Vector2 Vector2::operator- () { return Vector2(-x, -y); }
Vector2 Vector2::operator+ (Vector2 v) { return Vector2(x + v.x, y + v.y); }
Vector2 Vector2::operator- (Vector2 v) { return Vector2(x - v.x, y - v.y); }
float Vector2::operator* (Vector2 v) { return x * v.x + y * v.y; }
Vector2 Vector2::operator+= (Vector2 v) { return Vector2(x += v.x, y += v.y); }
Vector2 Vector2::operator-= (Vector2 v) { return Vector2(x -= v.x, y -= v.y); }
bool Vector2::operator== (Vector2 v) { return x == v.x && y == v.y; }
bool Vector2::operator!= (Vector2 v) { return x != v.x || y != v.y; }
Vector2::operator Vector3() { return Vector3(x, y, 0); }
Vector2::operator Vector4() { return Vector4(x, y, 0, 0); }
Vector2::operator Vector2Int() { return Vector2Int(x, y); }
Vector2::operator Vector3Int() { return Vector3Int(x, y, 0); }
Vector2::operator Vector4Int() { return Vector4Int(x, y, 0, 0); }

float Vector2::Distance(void) { return sqrtf(x * x + y * y); }
float Vector2::DistanceSq(void) { return x * x + y * y; }
Vector2 Vector2::Normal(void) { return Vector2(y, -x); }
Vector2 Vector2::NormalRev(void) { return Vector2(-y, x); }
Vector2 Vector2::Normalize(void) { return *this * (1 / Distance()); }
float Vector2::Dot(Vector2 a, Vector2 b) { return a.x * b.x + a.y * b.y; }
float Vector2::Cross(Vector2 a, Vector2 b) { return a.x * b.y - a.y * b.x; }
float Vector2::Distance(Vector2 a, Vector2 b) { return (b - a).Distance(); }
float Vector2::DistanceSq(Vector2 a, Vector2 b) { return (b - a).DistanceSq(); }



Vector3 Vector3::operator+ () { return Vector3(+x, +y, +z); }
Vector3 Vector3::operator- () { return Vector3(-x, -y, -z); }
Vector3 Vector3::operator+ (Vector3 v) { return Vector3(x + v.x, y + v.y, z + v.z); }
Vector3 Vector3::operator- (Vector3 v) { return Vector3(x - v.x, y - v.y, z - v.z); }
float Vector3::operator* (Vector3 v) { return x * v.x + y * v.y; }
Vector3 Vector3::operator+= (Vector3 v) { return Vector3(x += v.x, y += v.y, z += v.z); }
Vector3 Vector3::operator-= (Vector3 v) { return Vector3(x -= v.x, y -= v.y, z -= v.z); }
bool Vector3::operator== (Vector3 v) { return x == v.x && y == v.y && z == v.z; }
bool Vector3::operator!= (Vector3 v) { return x != v.x || y != v.y || z != v.z; }
Vector3::operator Vector4() { return Vector4(x, y, z, 0); }
Vector3::operator Vector3Int() { return Vector3Int(x, y, z); }
Vector3::operator Vector4Int() { return Vector4Int(x, y, z, 0); }

float Vector3::Distance(void) { return sqrtf(x * x + y * y + z * z); }
Vector3 Vector3::Normalize(void) { return *this * (1 / Distance()); }
Vector2 Vector3::Compress(void) { return Vector2(x, y); }
float Vector3::Dot(Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vector3 Vector3::Cross(Vector3 a, Vector3 b) { return Vector3(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x, a.x * b.y - a.y * b.x); }
float Vector3::Distance(Vector3 a, Vector3 b) { return (b - a).Distance(); }



Vector4 Vector4::operator+ () { return Vector4(+x, +y, +z, +w); }
Vector4 Vector4::operator- () { return Vector4(-x, -y, -z, -w); }
Vector4 Vector4::operator+ (Vector4 v) { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
Vector4 Vector4::operator- (Vector4 v) { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
float Vector4::operator* (Vector4 v) { return x * v.x + y * v.y; }
Vector4 Vector4::operator+= (Vector4 v) { return Vector4(x += v.x, y += v.y, z += v.z, w += v.w); }
Vector4 Vector4::operator-= (Vector4 v) { return Vector4(x -= v.x, y -= v.y, z -= v.z, w -= v.w); }
bool Vector4::operator== (Vector4 v) { return x == v.x && y == v.y && z == v.z && w == v.w; }
bool Vector4::operator!= (Vector4 v) { return x != v.x || y != v.y || z != v.z || w != v.w; }
Vector4::operator Vector4Int() { return Vector4Int(x, y, z, w); }

Vector3 Vector4::Compress(void) { return Vector3(x, y, z); }






Vector2Int Vector2Int::operator+ (Vector2Int v) { return Vector2Int(x + v.x, y + v.y); }
Vector2Int Vector2Int::operator- (Vector2Int v) { return Vector2Int(x - v.x, y - v.y); }
int Vector2Int::operator* (Vector2Int v) { return x * v.x + y * v.y; }
Vector2Int Vector2Int::operator+= (Vector2Int v) { return Vector2Int(x += v.x, y += v.y); }
Vector2Int Vector2Int::operator-= (Vector2Int v) { return Vector2Int(x -= v.x, y -= v.y); }
bool Vector2Int::operator== (Vector2Int v) { return x == v.x && y == v.y; }
bool Vector2Int::operator!= (Vector2Int v) { return x != v.x || y != v.y; }
Vector2Int::operator Vector2() { return Vector2(x, y); }
Vector2Int::operator Vector3() { return Vector3(x, y, 0); }
Vector2Int::operator Vector4() { return Vector4(x, y, 0, 0); }
Vector2Int::operator Vector3Int() { return Vector3Int(x, y, 0); }
Vector2Int::operator Vector4Int() { return Vector4Int(x, y, 0, 0); }

int Vector2Int::Distance(void) { return sqrtf(x * x + y * y); }
Vector2Int Vector2Int::Normal(void) { return Vector2Int(y, -x); }
Vector2Int Vector2Int::Normalize(void) { return *this * (1 / Distance()); }
int Vector2Int::Dot(Vector2Int a, Vector2Int b) { return a.x * b.x + a.y * b.y; }
int Vector2Int::Cross(Vector2Int a, Vector2Int b) { return a.x * b.y - a.y * b.x; }
int Vector2Int::Distance(Vector2Int a, Vector2Int b) { return (b - a).Distance(); }



Vector3Int Vector3Int::operator+ (Vector3Int v) { return Vector3Int(x + v.x, y + v.y, z + v.z); }
Vector3Int Vector3Int::operator- (Vector3Int v) { return Vector3Int(x - v.x, y - v.y, z - v.z); }
int Vector3Int::operator* (Vector3Int v) { return x * v.x + y * v.y; }
Vector3Int Vector3Int::operator+= (Vector3Int v) { return Vector3Int(x += v.x, y += v.y, z += v.z); }
Vector3Int Vector3Int::operator-= (Vector3Int v) { return Vector3Int(x -= v.x, y -= v.y, z -= v.z); }
bool Vector3Int::operator== (Vector3Int v) { return x == v.x && y == v.y && z == v.z; }
bool Vector3Int::operator!= (Vector3Int v) { return x != v.x || y != v.y || z != v.z; }
Vector3Int::operator Vector3() { return Vector3(x, y, 0); }
Vector3Int::operator Vector4() { return Vector4(x, y, 0, 0); }
Vector3Int::operator Vector4Int() { return Vector4Int(x, y, 0, 0); }

int Vector3Int::Distance(void) { return sqrtf(x * x + y * y + z * z); }
Vector3Int Vector3Int::Normalize(void) { return *this * (1 / Distance()); }
Vector2Int Vector3Int::Compress(void) { return Vector2Int(x, y); }
int Vector3Int::Dot(Vector3Int a, Vector3Int b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vector3Int Vector3Int::Cross(Vector3Int a, Vector3Int b) { return Vector3Int(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x, a.x * b.y - a.y * b.x); }
int Vector3Int::Distance(Vector3Int a, Vector3Int b) { return (b - a).Distance(); }



Vector4Int Vector4Int::operator+ (Vector4Int v) { return Vector4Int(x + v.x, y + v.y, z + v.z, w + v.w); }
Vector4Int Vector4Int::operator- (Vector4Int v) { return Vector4Int(x - v.x, y - v.y, z - v.z, w - v.w); }
int Vector4Int::operator* (Vector4Int v) { return x * v.x + y * v.y; }
Vector4Int Vector4Int::operator+= (Vector4Int v) { return Vector4Int(x += v.x, y += v.y, z += v.z, w += v.w); }
Vector4Int Vector4Int::operator-= (Vector4Int v) { return Vector4Int(x -= v.x, y -= v.y, z -= v.z, w -= v.w); }
bool Vector4Int::operator== (Vector4Int v) { return x == v.x && y == v.y && z == v.z && w == v.w; }
bool Vector4Int::operator!= (Vector4Int v) { return x != v.x || y != v.y || z != v.z || w != v.w; }
Vector4Int::operator Vector4() { return Vector4(x, y, 0, 0); }

Vector3Int Vector4Int::Compress(void) { return Vector3Int(x, y); }
