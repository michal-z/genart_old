#pragma once

typedef struct VECTOR2 {
    f32 x, y;
} VECTOR2;

typedef struct VECTOR3 {
    f32 x, y, z;
} VECTOR3;

typedef struct VECTOR4 {
    f32 x, y, z, w;
} VECTOR4;

inline VECTOR2 Vec2_Set(f32 x, f32 y) {
    VECTOR2 r = { x, y };
    return r;
}

inline VECTOR3 Vec3_Set(f32 x, f32 y, f32 z) {
    VECTOR3 r = { x, y, z };
    return r;
}

inline VECTOR4 Vec4_Set(f32 x, f32 y, f32 z, f32 w) {
    VECTOR4 r = { x, y, z, w };
    return r;
}

inline VECTOR2 Vec2_Scale(VECTOR2 a, f32 b) {
    VECTOR2 r = { a.x * b, a.y * b };
    return r;
}

inline VECTOR3 Vec3_Scale(VECTOR3 a, f32 b) {
    VECTOR3 r = { a.x * b, a.y * b, a.z * b };
    return r;
}

inline VECTOR4 Vec4_Scale(VECTOR4 a, f32 b) {
    VECTOR4 r = { a.x * b, a.y * b, a.z * b, a.w * b };
    return r;
}

inline VECTOR2 Vec2_Add(VECTOR2 a, VECTOR2 b) {
    VECTOR2 r = { a.x + b.x, a.y + b.y };
    return r;
}

inline VECTOR3 Vec3_Add(VECTOR3 a, VECTOR3 b) {
    VECTOR3 r = { a.x + b.x, a.y + b.y, a.z + b.z };
    return r;
}

inline VECTOR4 Vec4_Add(VECTOR4 a, VECTOR4 b) {
    VECTOR4 r = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
    return r;
}

inline VECTOR2 Vec2_Sub(VECTOR2 a, VECTOR2 b) {
    VECTOR2 r = { a.x - b.x, a.y - b.y };
    return r;
}

inline VECTOR3 Vec3_Sub(VECTOR3 a, VECTOR3 b) {
    VECTOR3 r = { a.x - b.x, a.y - b.y, a.z - b.z };
    return r;
}

inline VECTOR4 Vec4_Sub(VECTOR4 a, VECTOR4 b) {
    VECTOR4 r = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
    return r;
}

inline f32 Vec2_Dot(VECTOR2 a, VECTOR2 b) {
    return a.x * b.x + a.y * b.y;
}

inline f32 Vec3_Dot(VECTOR3 a, VECTOR3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline f32 Vec4_Dot(VECTOR4 a, VECTOR4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline f32 Vec2_Length(VECTOR2 a) {
    return sqrtf(Vec2_Dot(a, a));
}

inline f32 Vec3_Length(VECTOR3 a) {
    return sqrtf(Vec3_Dot(a, a));
}

inline f32 Vec4_Length(VECTOR4 a) {
    return sqrtf(Vec4_Dot(a, a));
}

inline VECTOR2 Vec2_Normalize(VECTOR2 a) {
    f32 len = Vec2_Length(a);
    assert(len != 0.0f);
    f32 rcplen = 1.0f / len;
    return Vec2_Scale(a, rcplen);
}

inline VECTOR3 Vec3_Normalize(VECTOR3 a) {
    f32 len = Vec3_Length(a);
    assert(len != 0.0f);
    f32 rcplen = 1.0f / len;
    return Vec3_Scale(a, rcplen);
}

inline VECTOR4 Vec4_Normalize(VECTOR4 a) {
    f32 len = Vec4_Length(a);
    assert(len != 0.0f);
    f32 rcplen = 1.0f / len;
    return Vec4_Scale(a, rcplen);
}

inline VECTOR3 Vec3_Cross(VECTOR3 a, VECTOR3 b) {
    VECTOR3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

inline f32 Vec2_Distance(VECTOR2 a, VECTOR2 b) {
    return Vec2_Length(Vec2_Sub(a, b));
}

inline f32 Vec3_Distance(VECTOR3 a, VECTOR3 b) {
    return Vec3_Length(Vec3_Sub(a, b));
}

inline f32 Vec4_Distance(VECTOR4 a, VECTOR4 b) {
    return Vec4_Length(Vec4_Sub(a, b));
}
