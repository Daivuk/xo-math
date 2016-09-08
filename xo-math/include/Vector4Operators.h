#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector4Operators.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

#if XO_SSE

#if defined IDX_X
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define IDX_X 0
#   endif
#if defined IDX_Y
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define IDX_Y 1
#   endif
#if defined IDX_Z
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define IDX_Z 2
#   endif
#if defined IDX_W
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define IDX_W 3
#   endif

#endif

#if XO_SSE
Vector4::operator const __m128&() const {
    return m;
}
#endif

float& Vector4::operator [](int i) {
    return f[i];
}

const float& Vector4::operator [](int i) const {
    return f[i];
}

Vector4 Vector4::operator -() const {
#if XO_SSE
    return Vector4(_mm_mul_ps(m, sse::NegativeOne));
#else
    return Vector4(-x, -y, -z, -w);
#endif
}

Vector4 Vector4::operator ~() const {
#if XO_SSE
    return Vector3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_X, IDX_Y, IDX_Z, IDX_W)));
#else
    return Vector4(w, z, y, x);
#endif
}

const Vector4& Vector4::operator += (const Vector4& v) {
#if XO_SSE
    m = _mm_add_ps(m, v.m);
#else
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
#endif
    return *this;
}

const Vector4& Vector4::operator += (float v) {
#if XO_SSE
    m = _mm_add_ps(m, _mm_set_ps1(v));
#else
    x += v;
    y += v;
    z += v;
    w += v;
#endif
    return *this;
}

const Vector4& Vector4::operator += (double v)          { return (*this) += (float)v; }
const Vector4& Vector4::operator += (int v)             { return (*this) += (float)v; }
const Vector4& Vector4::operator += (const Vector2& v)  { return (*this) += Vector4(v); }
const Vector4& Vector4::operator += (const Vector3& v)  { return (*this) += Vector4(v); }

const Vector4& Vector4::operator -= (const Vector4& v) {
#if XO_SSE
    m = _mm_sub_ps(m, v.m);
#else
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
#endif
    return *this;
}

const Vector4& Vector4::operator -= (float v) {
#if XO_SSE
    m = _mm_sub_ps(m, _mm_set_ps1(v));
#else
    x -= v;
    y -= v;
    z -= v;
    w -= v;
#endif
    return *this;
}

const Vector4& Vector4::operator -= (double v)          { return (*this) -= (float)v; }
const Vector4& Vector4::operator -= (int v)             { return (*this) -= (float)v; }
const Vector4& Vector4::operator -= (const Vector2& v)  { return (*this) -= Vector4(v); }
const Vector4& Vector4::operator -= (const Vector3& v)  { return (*this) -= Vector4(v); }

const Vector4& Vector4::operator *= (const Vector4& v) {
#if XO_SSE
    m = _mm_mul_ps(m, v.m);
#else
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
#endif
    return *this;
}

const Vector4& Vector4::operator *= (float v) {
#if XO_SSE
    m = _mm_mul_ps(m, _mm_set_ps1(v));
#else
    x *= v;
    y *= v;
    z *= v;
    w *= v;
#endif
    return *this;
}

const Vector4& Vector4::operator *= (const Matrix4x4& m) {
    return Set(((*this) * m.r[0]).Sum(), ((*this) * m.r[1]).Sum(), ((*this) * m.r[2]).Sum(), ((*this) * m.r[3]).Sum());
}

const Vector4& Vector4::operator *= (double v)          { return (*this) *= (float)v; }
const Vector4& Vector4::operator *= (int v)             { return (*this) *= (float)v; }
const Vector4& Vector4::operator *= (const Vector2& v)  { return (*this) *= Vector4(v); }
const Vector4& Vector4::operator *= (const Vector3& v)  { return (*this) *= Vector4(v); }

#if defined(XO_NO_INVERSE_DIVISION)
const Vector4& Vector4::operator /= (const Vector4& v) {
#   if XO_SSE
    // see: https://software.intel.com/sites/landingpage/IntrinsicsGuide
    // see: https://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput

    // _mm_div_ps
    // -----------------------------------
    // Architecture    Latency Throughput
    // Haswell         13      5
    // Ivy Bridge      13      6
    // Sandy Bridge    14      14
    // Westmere        14      12
    // Nehalem         14      12
    m = _mm_div_ps(m, v.m);
#   else
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
#   endif
    return *this;
}

const Vector4& Vector4::operator /= (float v) {
#   if XO_SSE
    m = _mm_div_ps(m, _mm_set_ps1(v));
#   else
    x /= v;
    y /= v;
    z /= v;
    w /= v;
#   endif
    return *this;
}
#else

const Vector4& Vector4::operator /= (const Vector4& v) {
#   if XO_SSE
    // see: https://software.intel.com/sites/landingpage/IntrinsicsGuide
    // see: https://software.intel.com/en-us/articles/measuring-instruction-latency-and-throughput

    // _mm_rcp_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        1
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        3        2
    // Nehalem         3        2

    // _mm_mul_ps
    // -----------------------------------
    // Architecture    Latency  Throughput
    // Haswell         5        0.5
    // Ivy Bridge      5        1
    // Sandy Bridge    5        1
    // Westmere        4        1
    // Nehalem         4        1
    m = _mm_mul_ps(m, _mm_rcp_ps(v.m));
#   else
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
#endif
    return *this;
}

const Vector4& Vector4::operator /= (float v) { 
#   if XO_SSE
    m = _mm_mul_ps(m, _mm_set_ps1(1.0f/v));
#   else
    v = 1.0f / v;
    x *= v;
    y *= v;
    z *= v;
    w *= v;
#   endif
    return *this;
}
#endif
const Vector4& Vector4::operator /= (double v)          { return (*this) /= float(v); }
const Vector4& Vector4::operator /= (int v)             { return (*this) /= float(v); }
const Vector4& Vector4::operator /= (const Vector2& v)  { return (*this) /= Vector4(v); }
const Vector4& Vector4::operator /= (const Vector3& v)  { return (*this) /= Vector4(v); }

Vector4 Vector4::operator + (const Vector4& v) const    { return Vector4(*this) += v; }
Vector4 Vector4::operator + (float v) const             { return Vector4(*this) += v; }
Vector4 Vector4::operator + (double v) const            { return Vector4(*this) += v; }
Vector4 Vector4::operator + (int v) const               { return Vector4(*this) += v; }
Vector4 Vector4::operator + (const Vector2& v) const    { return Vector4(*this) += v; }
Vector4 Vector4::operator + (const Vector3& v) const    { return Vector4(*this) += v; }

Vector4 Vector4::operator - (const Vector4& v) const    { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (float v) const             { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (double v) const            { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (int v) const               { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (const Vector2& v) const    { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (const Vector3& v) const    { return Vector4(*this) -= v; }

Vector4 Vector4::operator * (const Vector4& v) const    { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (float v) const             { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (double v) const            { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (int v) const               { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (const Vector2& v) const    { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (const Vector3& v) const    { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (const Matrix4x4& m) const  { return Vector4(*this) *= m; }

Vector4 Vector4::operator / (const Vector4& v) const    { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (float v) const             { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (double v) const            { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (int v) const               { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (const Vector2& v) const    { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (const Vector3& v) const    { return Vector4(*this) /= v; }

bool Vector4::operator < (const Vector4& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector4::operator < (float v) const                { return MagnitudeSquared() < (v * v); }
bool Vector4::operator < (double v) const               { return MagnitudeSquared() < (float)(v * v); }
bool Vector4::operator < (int v) const                  { return MagnitudeSquared() < (float)(v * v); }
bool Vector4::operator < (const Vector2& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector4::operator < (const Vector3& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector4::operator <= (const Vector4& v) const      { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector4::operator <= (float v) const               { return MagnitudeSquared() <= (v * v); }
bool Vector4::operator <= (double v) const              { return MagnitudeSquared() <= (float)(v * v); }
bool Vector4::operator <= (int v) const                 { return MagnitudeSquared() <= (float)(v * v); }
bool Vector4::operator <= (const Vector2& v) const      { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector4::operator <= (const Vector3& v) const      { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector4::operator > (const Vector4& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector4::operator > (float v) const                { return MagnitudeSquared() > (v * v); }
bool Vector4::operator > (double v) const               { return MagnitudeSquared() > (float)(v * v); }
bool Vector4::operator > (int v) const                  { return MagnitudeSquared() > (float)(v * v); }
bool Vector4::operator > (const Vector2& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector4::operator > (const Vector3& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector4::operator >= (const Vector4& v) const      { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector4::operator >= (float v) const               { return MagnitudeSquared() >= (v * v); }
bool Vector4::operator >= (double v) const              { return MagnitudeSquared() >= (float)(v * v); }
bool Vector4::operator >= (int v) const                 { return MagnitudeSquared() >= (float)(v * v); }
bool Vector4::operator >= (const Vector2& v) const      { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector4::operator >= (const Vector3& v) const      { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector4::operator == (const Vector4& v) const {
#   if XO_SSE2
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.m, m)), sse::Epsilon)) & 0b1111) == 0b1111;
#   elif XO_SSE
    // TODO: find a faster way with SSE to do a 'close enough' check.
    // I'm not sure if there's a way to do the sign bit masking like we have in sse::Abs to acomplish
    // what we're doing in SSE2
    return 
        CloseEnough(x, v.x, sse::SSEFloatEpsilon) && 
        CloseEnough(y, v.y, sse::SSEFloatEpsilon) && 
        CloseEnough(z, v.z, sse::SSEFloatEpsilon) &&
        CloseEnough(w, v.w, sse::SSEFloatEpsilon);
#   else
    return 
        CloseEnough(x, v.x, Epsilon) && 
        CloseEnough(y, v.y, Epsilon) && 
        CloseEnough(z, v.z, Epsilon) &&
        CloseEnough(w, v.w, Epsilon);
#   endif
}
bool Vector4::operator == (float v) const           { return CloseEnough(MagnitudeSquared(), v * v, Epsilon); }
bool Vector4::operator == (double v) const          { return CloseEnough(MagnitudeSquared(), (float)(v * v), Epsilon); }
bool Vector4::operator == (int v) const             { return CloseEnough(MagnitudeSquared(), (float)(v * v), Epsilon); }

bool Vector4::operator == (const Vector2& v) const {
#   if XO_SSE
    // Todo: check that this is actually faster.
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(m, _mm_set_ps(0.0f, 0.0f, v.y, v.x))), sse::Epsilon)) & 0b0011) == 0b0011;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}

bool Vector4::operator == (const Vector3& v) const {
#   if XO_SSE
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.m, m)), sse::Epsilon)) & 0b0111) == 0b0111;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon) && CloseEnough(z, v.z, Epsilon);
#   endif
}

bool Vector4::operator != (const Vector4& v) const  { return !((*this) == v); }
bool Vector4::operator != (float v) const           { return !((*this) == v); }
bool Vector4::operator != (double v) const          { return !((*this) == v); }
bool Vector4::operator != (int v) const             { return !((*this) == v); }
bool Vector4::operator != (const Vector2& v) const  { return !((*this) == v); }
bool Vector4::operator != (const Vector3& v) const  { return !((*this) == v); }

#undef IDX_X
#undef IDX_Y
#undef IDX_Z
#undef IDX_W

XOMATH_END_XO_NS

#endif