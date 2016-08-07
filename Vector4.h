#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector4.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS
// TODO: align new/delete
class _MM_ALIGN16 Vector4 {
    XO_IF_SIMD (
        static const unsigned IDX_X = 0;
        static const unsigned IDX_Y = 1;
        static const unsigned IDX_Z = 2;
        static const unsigned IDX_W = 3;
    )

public:
    constexpr static const float Epsilon = FloatEpsilon+FloatEpsilon+FloatEpsilon+FloatEpsilon;

    XOMATH_INLINE Vector4() :
        XO_IF_SIMD (
            m(_mm_setzero_ps())
        ) XO_IFN_SIMD (
            x(0.0f), y(0.0f), z(0.0f), w(0.0f)
        ) { }
    XOMATH_INLINE Vector4(float f) :
        XO_IF_SIMD (
            m(_mm_set1_ps(f))
        ) XO_IFN_SIMD (
            x(f), y(f), z(f), w(f)
        ) { }
    XOMATH_INLINE Vector4(float x, float y, float z, float w) :
        XO_IF_SIMD (
            m(_mm_set_ps(w, z, y, x))
        ) XO_IFN_SIMD (
            x(x), y(y), z(z), w(w)
        ) { }
    XOMATH_INLINE Vector4(const Vector4& vec) :
        XO_IF_SIMD (
            m(vec.m)
        ) XO_IFN_SIMD (
            x(vec.x), y(vec.y), z(vec.z), w(vec.w)
        ) { }

#if !defined(XO_NO_SIMD)
    XOMATH_INLINE Vector4(const __m128& vec) : m(vec) { }
#endif

    Vector4(const class Vector2& v);
    Vector4(const class Vector3& v);

    XOMATH_INLINE float& XOMATH_FAST(operator [](int i)) {
        return f[i];
    }

    XOMATH_INLINE const float& XOMATH_FAST(operator [](int i) const) {
        return f[i];
    }

    XOMATH_INLINE void XOMATH_FAST(Set(float x, float y, float z, float w)) {
        XO_IF_SIMD (
            m = _mm_set_ps(w, z, y, x);
        ) XO_IFN_SIMD (
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        )
    }

    XOMATH_INLINE void XOMATH_FAST(Set(float f)) {
        XO_IF_SIMD (
            m = _mm_set1_ps(f);
        ) XO_IFN_SIMD (
            this->x = f;
            this->y = f;
            this->z = f;
            this->w = w;
        )
    }

    XOMATH_INLINE void XOMATH_FAST(Set(const Vector4& vec)) {
        XO_IF_SIMD (
            m = vec.m;
        ) XO_IFN_SIMD (
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
            this->w = vec.w;
        )
    }

#if !defined(XO_NO_SIMD)
    XOMATH_INLINE void XOMATH_FAST(Set(const __m128& vec)) {
        m = vec;
    }
#endif

    XOMATH_INLINE void XOMATH_FAST(Get(float& x, float& y, float& z, float& w) const) {
        x = this->x;
        y = this->y;
        z = this->z;
        w = this->w;
    }

    XOMATH_INLINE void XOMATH_FAST(Get(float* f) const) {
        XO_IF_SIMD (
            _mm_store_ps(f, m);
        ) XO_IFN_SIMD (
            f[0] = this->x;
            f[1] = this->y;
            f[2] = this->z;
            f[3] = this->w;
        )
    }

    VEC4D_SIMPLE_OP(+, _mm_add_ps)
    VEC4D_SIMPLE_OP(-, _mm_sub_ps)
    VEC4D_SIMPLE_OP(*, _mm_mul_ps)

    VEC4D_SIMPLE_OP_ADD(+=, +, _mm_add_ps)
    VEC4D_SIMPLE_OP_ADD(-=, -,  _mm_sub_ps)
    VEC4D_SIMPLE_OP_ADD(*=, *, _mm_mul_ps)

#if !defined(XO_NO_INVERSE_DIVISION)
    XOMATH_INLINE Vector4 XOMATH_FAST(operator / (const Vector4& v) const) {
        XO_IF_SIMD (
            return Vector4(_mm_div_ps(m, v.m));
        ) XO_IFN_SIMD (
            return Vector4(x/v.x, y/v.y, z/v.z, w/v.w);
        )
    } // no choice here, just divide each
    XOMATH_INLINE Vector4 XOMATH_FAST(operator / (float v) const)       { return (*this) * (1.0f/v); }
    XOMATH_INLINE Vector4 XOMATH_FAST(operator / (double v) const)      { return (*this) / (float)v; }
    XOMATH_INLINE Vector4 XOMATH_FAST(operator / (int v) const)         { return (*this) / (float)v; }
    XOMATH_INLINE Vector4 XOMATH_FAST(operator / (const class  Vector2& v) const);
    XOMATH_INLINE Vector4 XOMATH_FAST(operator / (const class  Vector3& v) const);
    XOMATH_INLINE const Vector4& XOMATH_FAST(operator /= (const Vector4& v)) {
        XO_IF_SIMD (
            m = _mm_div_ps(m, v.m);
        ) XO_IFN_SIMD (
            x /= v.x; 
            y /= v.y; 
            z /= v.z;
        )
        return *this;
    }
    XOMATH_INLINE const Vector4& XOMATH_FAST(operator /= (float v))     { return (*this) *= 1.0f / v; }
    XOMATH_INLINE const Vector4& XOMATH_FAST(operator /= (double v))    { return (*this) /= (float)v; }
    XOMATH_INLINE const Vector4& XOMATH_FAST(operator /= (int v))       { return (*this) /= (float)v; }
    XOMATH_INLINE const Vector4& XOMATH_FAST(operator /= (const class Vector2& v));
    XOMATH_INLINE const Vector4& XOMATH_FAST(operator /= (const class Vector3& v));
#else
    // scalar division is slower, so we try to avoid it unless XO_NO_INVERSE_DIVISION is defined.
    VEC4D_SIMPLE_OP(/ , _mm_div_ps)
    VEC4D_SIMPLE_OP_ADD(/=, / , _mm_div_ps)
#endif
    
    XOMATH_INLINE Vector4 operator - () const {
        XO_IF_SIMD (
            static const __m128 anti = _mm_set1_ps(-1.0f);
            return Vector4(_mm_mul_ps(m, anti));
        ) XO_IFN_SIMD (
            return Vector4(-x, -y, -z, -w);
        )
    }

    XOMATH_INLINE Vector4 operator ~ () const {
        XO_IF_SIMD (
            return Vector4(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_X, IDX_Y, IDX_Z, IDX_W)));
        ) XO_IFN_SIMD (
            return Vector4(w, z, y, x);
        )
    }

    XOMATH_INLINE Vector4 ZYX() const {
        return ~(*this);
    }

    XOMATH_INLINE float Sum() const {
        XO_IF_SIMD (
            auto s = _mm_hadd_ps(m, m);
            s = _mm_hadd_ps(s, s);
            return _mm_cvtss_f32(s);
        ) XO_IFN_SIMD (
            return x+y+z+w;
        )
    }

    XOMATH_INLINE float MagnitudeSquared() const {
        XO_IF_SIMD (
            auto square = _mm_mul_ps(m, m);
            square = _mm_hadd_ps(square, square);
            square = _mm_hadd_ps(square, square);
            return _mm_cvtss_f32(square);
        ) XO_IFN_SIMD (
            return (x*x) + (y*y) + (z*z) + (w*w);
        )
    }

    XOMATH_INLINE float Magnitude() const {
        return Sqrt(MagnitudeSquared());
    }

    XOMATH_INLINE Vector4 Normalized() const {
        float magnitude = MagnitudeSquared();
        if (CloseEnough(magnitude, 1.0f, Epsilon))
            return *this; // already normalized
        if (CloseEnough(magnitude, 0.0f, Epsilon))
            return *this; // zero vec
        magnitude = Sqrt(magnitude);
        return (*this) / magnitude;
    }

    XOMATH_INLINE void Normalize() {
        (*this) = Normalized();
    }

    XOMATH_INLINE bool IsZero() const {
        return MagnitudeSquared() <= Epsilon;
    }

    XOMATH_INLINE bool IsNormalized() const {
      // todo: check closeness
        return MagnitudeSquared() - 1.0f <= Epsilon;
    }

    // Note: comparisons against another vector will compare the magnitude.
    // comparisons against a number will also compare against magnitude (ignoring sign)
    VEC4_COMPARE_OP(<)
    VEC4_COMPARE_OP(<=)
    VEC4_COMPARE_OP(>)
    VEC4_COMPARE_OP(>=)
    // Note: '== and !=' against another Vector4 will check for the exact X,Y,Z match.
    // '== and !=' against a number will check against the magnitude (ignoring sign)
    VEC4_COMPARE_CLOSE_OP(==, &&)
    VEC4_COMPARE_CLOSE_OP(!=, ||)


    XOMATH_INLINE static Vector4 XOMATH_FAST(Max(const Vector4& a, const Vector4& b)) {
        return a >= b ? a : b;
    }

    XOMATH_INLINE static Vector4 XOMATH_FAST(Min(const Vector4& a, const Vector4& b)) {
        return a <= b ? a : b;
    }

    XOMATH_INLINE static float XOMATH_FAST(Dot(const Vector4& a, const Vector4& b)) {
        XO_IF_SIMD (
            auto d = _mm_mul_ps(a.m, b.m);
            d = _mm_hadd_ps(d, d);
            d = _mm_hadd_ps(d, d);
            return _mm_cvtss_f32(d);
        ) XO_IFN_SIMD (
            return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
        )
    }

    XOMATH_INLINE static Vector4 XOMATH_FAST(Cross(const Vector4& a, const Vector4& b)) {
        XO_IF_SIMD (
            // Todo: There's a trick to do this with three shuffles. Look into that.
            auto l = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)));
            auto r = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)));
            return Vector4(_mm_sub_ps(l, r));
        ) XO_IFN_SIMD (
            return Vector4((a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x));
        )
    }

    XOMATH_INLINE static float XOMATH_FAST(AngleRadians(const Vector4& a, const Vector4& b)) {
        XO_IF_SIMD (
            auto cross = Cross(a, b).m;
            cross = _mm_mul_ps(cross, cross);
            cross = _mm_hadd_ps(cross, cross);
            cross = _mm_hadd_ps(cross, cross);
            return Atan2(Sqrt(_mm_cvtss_f32(cross)), Dot(a, b));
        ) XO_IFN_SIMD (
            auto cross = Cross(a, b);
            cross *= cross;
            return Atan2(Sqrt(cross.x + cross.y + cross.z), Dot(a, b));
        )
    }

    XOMATH_INLINE static float XOMATH_FAST(AngleDegrees(const Vector4& a, const Vector4& b)) {
        return AngleRadians(a, b) * Rad2Deg;
    }

    XOMATH_INLINE static float XOMATH_FAST(DistanceSquared(const Vector4& a, const Vector4& b)) {
        return (b - a).MagnitudeSquared();
    }

    XOMATH_INLINE static float XOMATH_FAST(Distance(const Vector4&a, const Vector4&b)) {
        return (b - a).Magnitude();
    }

    XOMATH_INLINE static Vector4 XOMATH_FAST(Lerp(const Vector4& a, const Vector4& b, float t)) {
        return a + ((b - a) * t);
    }

    XOMATH_INLINE float XOMATH_FAST(Dot(const Vector4& v) const)                                { return Dot(*this, v); }
    XOMATH_INLINE Vector4 XOMATH_FAST(Cross(const Vector4& v) const)                            { return Cross(*this, v); }
    XOMATH_INLINE float XOMATH_FAST(AngleRadians(const Vector4& v) const)                       { return AngleRadians(*this, v); }
    XOMATH_INLINE float XOMATH_FAST(AngleDegrees(const Vector4& v) const)                       { return AngleDegrees(*this, v); }
    XOMATH_INLINE float XOMATH_FAST(DistanceSquared(const Vector4& v) const)                    { return DistanceSquared(*this, v); }
    XOMATH_INLINE float XOMATH_FAST(Distance(const Vector4& v) const)                           { return Distance(*this, v); }
    XOMATH_INLINE Vector4 XOMATH_FAST(Lerp(const Vector4& v, float t) const)                    { return Lerp(*this, v, t); }

    friend std::ostream& operator <<(std::ostream& os, const Vector4& v) {
        XO_IF_SIMD (
            os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
        ) XO_IFN_SIMD(
            os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", mag:" << v.Magnitude() << ")";
        )
        return os;
    }

    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
#if !defined(XO_NO_SIMD)
        __m128 m;
#endif
    };
};

XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL