#pragma once

struct VECTOR2 {
    f32 x, y;
};

struct VECTOR3 {
    f32 x, y, z;
};

struct VECTOR4 {
    f32 x, y, z, w;
};

inline VECTOR2 operator*(f32 a, VECTOR2 b) {
    return VECTOR2{ a * b.x, a * b.y };
}

inline VECTOR2 operator*(VECTOR2 a, f32 b) {
    return VECTOR2{ a.x * b, a.y * b };
}

inline VECTOR3 operator*(f32 a, VECTOR3 b) {
    return VECTOR3{ a * b.x, a * b.y, a * b.z };
}

inline VECTOR3 operator*(VECTOR3 a, f32 b) {
    return VECTOR3{ a.x * b, a.y * b, a.z * b };
}

inline VECTOR4 operator*(f32 a, VECTOR4 b) {
    return VECTOR4{ a * b.x, a * b.y, a * b.z, a * b.w };
}

inline VECTOR4 operator*(VECTOR4 a, f32 b) {
    return VECTOR4{ a.x * b, a.y * b, a.z * b, a.w * b };
}

inline VECTOR2 operator+(VECTOR2 a, VECTOR2 b) {
    return VECTOR2{ a.x + b.x, a.y + b.y };
}

inline VECTOR3 operator+(VECTOR3 a, VECTOR3 b) {
    return VECTOR3{ a.x + b.x, a.y + b.y, a.z + b.z };
}

inline VECTOR4 operator+(VECTOR4 a, VECTOR4 b) {
    return VECTOR4{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline VECTOR2 operator-(VECTOR2 a, VECTOR2 b) {
    return VECTOR2{ a.x - b.x, a.y - b.y };
}

inline VECTOR3 operator-(VECTOR3 a, VECTOR3 b) {
    return VECTOR3{ a.x - b.x, a.y - b.y, a.z - b.z };
}

inline VECTOR4 operator-(VECTOR4 a, VECTOR4 b) {
    return VECTOR4{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline f32 Dot(VECTOR2 a, VECTOR2 b) {
    return a.x * b.x + a.y * b.y;
}

inline f32 Dot(VECTOR3 a, VECTOR3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline f32 Dot(VECTOR4 a, VECTOR4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline f32 Length(VECTOR2 a) {
    return sqrtf(Dot(a, a));
}

inline f32 Length(VECTOR3 a) {
    return sqrtf(Dot(a, a));
}

inline f32 Length(VECTOR4 a) {
    return sqrtf(Dot(a, a));
}

inline VECTOR2 Normalize(VECTOR2 a) {
    f32 len = Length(a);
    assert(len != 0.0f);
    return 1.0f / len * a;
}

inline VECTOR3 Normalize(VECTOR3 a) {
    f32 len = Length(a);
    assert(len != 0.0f);
    return 1.0f / len * a;
}

inline VECTOR4 Normalize(VECTOR4 a) {
    f32 len = Length(a);
    assert(len != 0.0f);
    return 1.0f / len * a;
}

inline VECTOR3 Cross(VECTOR3 a, VECTOR3 b) {
    return VECTOR3{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

inline f32 Distance(VECTOR2 a, VECTOR2 b) {
    return Length(a - b);
}

inline f32 Distance(VECTOR3 a, VECTOR3 b) {
    return Length(a - b);
}

inline f32 Distance(VECTOR4 a, VECTOR4 b) {
    return Length(a - b);
}

inline f32 Min(f32 a, f32 b) {
    return a < b ? a : b;
}

inline f32 Max(f32 a, f32 b) {
    return a > b ? a : b;
}

inline f32 Clamp(f32 a, f32 min, f32 max) {
    return Min(Max(min, a), max);
}

inline f32 Saturate(f32 a) {
    return Min(Max(0.0f, a), 1.0f);
}
