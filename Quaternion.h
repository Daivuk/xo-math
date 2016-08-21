#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Quaternion.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

class _MM_ALIGN16 Quaternion {
#if XO_SSE
    static const unsigned IDX_X = 0;
    static const unsigned IDX_Y = 1;
    static const unsigned IDX_Z = 2;
    static const unsigned IDX_W = 3;
#endif
public:
    constexpr static const float Epsilon = FloatEpsilon * 4.0f;

    _XOINL Quaternion();
    _XOINL Quaternion(const Matrix4x4& m);
    _XOINL Quaternion(float x, float y, float z, float w);

    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;

    _XOINL const Quaternion& operator *= (const Quaternion& q);
    _XOINL Quaternion operator * (const Quaternion& q) const;

    _XOINL bool operator == (const Quaternion& q) const;
    _XOINL bool operator != (const Quaternion& q) const;

    _XOINL Quaternion Inverse() const;
    _XOINL const Quaternion& MakeInverse();

    _XOINL Quaternion Normalized() const;
    _XOINL const Quaternion& Normalize();

    _XOINL Quaternion Conjugate() const;
    _XOINL const Quaternion& MakeConjugate();

    _XOINL void GetAxisAngleRadians(Vector3& axis, float& radians) const;

    _XOINL static void RotationRadians(float x, float y, float z, Quaternion& outQuat);
    _XOINL static void RotationRadians(const Vector3& v, Quaternion& outQuat);
    _XOINL static void AxisAngleRadians(const Vector3& axis, float radians, Quaternion& outQuat);
    
    _XOINL static void LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Quaternion& outQuat);
    _XOINL static void LookAtFromPosition(const Vector3& from, const Vector3& to, Quaternion& outQuat);
    _XOINL static void LookAtFromDirection(const Vector3& direction, const Vector3& up, Quaternion& outQuat);
    _XOINL static void LookAtFromDirection(const Vector3& direction, Quaternion& outQuat);

    _XOINL static void Slerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat);
    _XOINL static void Lerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat);


    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
#if XO_SSE
        __m128 m;
#endif
    };

    static const Quaternion
        Identity, Zero;
};

const Quaternion Quaternion::Identity = { 0.0f, 0.0f, 0.0f, 1.0f };
const Quaternion Quaternion::Zero = { 0.0f, 0.0f, 0.0f, 0.0f };

XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL