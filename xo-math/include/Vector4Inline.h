// The MIT License (MIT)
//
// Copyright (c) 2016 Jared Thomson
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT 
// OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.

XOMATH_BEGIN_XO_NS();

#if defined(XO_SSE)

#if !defined(IDX_X)
#   define IDX_X 0
#endif

#if !defined(IDX_Y)
#   define IDX_Y 1
#endif

#if !defined(IDX_Z)
#   define IDX_Z 2
#endif

#if !defined(IDX_W)
#   define IDX_W 3
#endif

#endif

#if defined(XO_SSE)
Vector4::operator const __m128&() const {
    return xmm;
}
#endif

float& Vector4::operator [](int i) {
    return f[i];
}

const float& Vector4::operator [](int i) const {
    return f[i];
}

Vector4 Vector4::operator -() const {
#if defined(XO_SSE)
    return Vector4(_mm_mul_ps(xmm, sse::NegativeOne));
#else
    return Vector4(-x, -y, -z, -w);
#endif
}

Vector4 Vector4::operator ~() const {
#if defined(XO_SSE)
    return Vector3(_mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(IDX_X, IDX_Y, IDX_Z, IDX_W)));
#else
    return Vector4(w, z, y, x);
#endif
}

Vector4& Vector4::operator += (const Vector4& v) {
#if defined(XO_SSE)
    xmm = _mm_add_ps(xmm, v.xmm);
#else
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
#endif
    return *this;
}

Vector4& Vector4::operator += (float v) {
#if defined(XO_SSE)
    xmm = _mm_add_ps(xmm, _mm_set_ps1(v));
#else
    x += v;
    y += v;
    z += v;
    w += v;
#endif
    return *this;
}

Vector4& Vector4::operator += (double v)          { return (*this) += (float)v; }
Vector4& Vector4::operator += (int v)             { return (*this) += (float)v; }
Vector4& Vector4::operator += (const class Vector2& v)  { return (*this) += Vector4(v); }
Vector4& Vector4::operator += (const class Vector3& v)  { return (*this) += Vector4(v); }

Vector4& Vector4::operator -= (const Vector4& v) {
#if defined(XO_SSE)
    xmm = _mm_sub_ps(xmm, v.xmm);
#else
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
#endif
    return *this;
}

Vector4& Vector4::operator -= (float v) {
#if defined(XO_SSE)
    xmm = _mm_sub_ps(xmm, _mm_set_ps1(v));
#else
    x -= v;
    y -= v;
    z -= v;
    w -= v;
#endif
    return *this;
}

Vector4& Vector4::operator -= (double v)          { return (*this) -= (float)v; }
Vector4& Vector4::operator -= (int v)             { return (*this) -= (float)v; }
Vector4& Vector4::operator -= (const class Vector2& v)  { return (*this) -= Vector4(v); }
Vector4& Vector4::operator -= (const class Vector3& v)  { return (*this) -= Vector4(v); }

Vector4& Vector4::operator *= (const Vector4& v) {
#if defined(XO_SSE)
    xmm = _mm_mul_ps(xmm, v.xmm);
#else
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
#endif
    return *this;
}

Vector4& Vector4::operator *= (float v) {
#if defined(XO_SSE)
    xmm = _mm_mul_ps(xmm, _mm_set_ps1(v));
#else
    x *= v;
    y *= v;
    z *= v;
    w *= v;
#endif
    return *this;
}

Vector4& Vector4::operator *= (double v)          { return (*this) *= (float)v; }
Vector4& Vector4::operator *= (int v)             { return (*this) *= (float)v; }
Vector4& Vector4::operator *= (const class Vector2& v)  { return (*this) *= Vector4(v); }
Vector4& Vector4::operator *= (const class Vector3& v)  { return (*this) *= Vector4(v); }

#if defined(XO_NO_INVERSE_DIVISION)
Vector4& Vector4::operator /= (const Vector4& v) {
#   if defined(XO_SSE)
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
    xmm = _mm_div_ps(xmm, v.xmm);
#   else
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
#   endif
    return *this;
}

Vector4& Vector4::operator /= (float v) {
#   if defined(XO_SSE)
    xmm = _mm_div_ps(xmm, _mm_set_ps1(v));
#   else
    x /= v;
    y /= v;
    z /= v;
    w /= v;
#   endif
    return *this;
}
#else

Vector4& Vector4::operator /= (const Vector4& v) {
#   if defined(XO_SSE)
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
    xmm = _mm_mul_ps(xmm, _mm_rcp_ps(v.xmm));
#   else
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
#endif
    return *this;
}

Vector4& Vector4::operator /= (float v) { 
#   if defined(XO_SSE)
    xmm = _mm_mul_ps(xmm, _mm_set_ps1(1.0f/v));
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
Vector4& Vector4::operator /= (double v)          { return (*this) /= float(v); }
Vector4& Vector4::operator /= (int v)             { return (*this) /= float(v); }
Vector4& Vector4::operator /= (const class Vector2& v)  { return (*this) /= Vector4(v); }
Vector4& Vector4::operator /= (const class Vector3& v)  { return (*this) /= Vector4(v); }

Vector4 Vector4::operator + (const Vector4& v) const    { return Vector4(*this) += v; }
Vector4 Vector4::operator + (float v) const             { return Vector4(*this) += v; }
Vector4 Vector4::operator + (double v) const            { return Vector4(*this) += v; }
Vector4 Vector4::operator + (int v) const               { return Vector4(*this) += v; }
Vector4 Vector4::operator + (const class Vector2& v) const    { return Vector4(*this) += v; }
Vector4 Vector4::operator + (const class Vector3& v) const    { return Vector4(*this) += v; }

Vector4 Vector4::operator - (const Vector4& v) const    { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (float v) const             { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (double v) const            { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (int v) const               { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (const class Vector2& v) const    { return Vector4(*this) -= v; }
Vector4 Vector4::operator - (const class Vector3& v) const    { return Vector4(*this) -= v; }

Vector4 Vector4::operator * (const Vector4& v) const    { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (float v) const             { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (double v) const            { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (int v) const               { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (const class Vector2& v) const    { return Vector4(*this) *= v; }
Vector4 Vector4::operator * (const class Vector3& v) const    { return Vector4(*this) *= v; }

Vector4 Vector4::operator / (const Vector4& v) const    { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (float v) const             { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (double v) const            { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (int v) const               { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (const class Vector2& v) const    { return Vector4(*this) /= v; }
Vector4 Vector4::operator / (const class Vector3& v) const    { return Vector4(*this) /= v; }

bool Vector4::operator < (const Vector4& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector4::operator < (float v) const                { return MagnitudeSquared() < (v * v); }
bool Vector4::operator < (double v) const               { return MagnitudeSquared() < (float)(v * v); }
bool Vector4::operator < (int v) const                  { return MagnitudeSquared() < (float)(v * v); }
bool Vector4::operator < (const class Vector2& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector4::operator < (const class Vector3& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector4::operator <= (const Vector4& v) const      { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector4::operator <= (float v) const               { return MagnitudeSquared() <= (v * v); }
bool Vector4::operator <= (double v) const              { return MagnitudeSquared() <= (float)(v * v); }
bool Vector4::operator <= (int v) const                 { return MagnitudeSquared() <= (float)(v * v); }
bool Vector4::operator <= (const class Vector2& v) const      { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector4::operator <= (const class Vector3& v) const      { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector4::operator > (const Vector4& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector4::operator > (float v) const                { return MagnitudeSquared() > (v * v); }
bool Vector4::operator > (double v) const               { return MagnitudeSquared() > (float)(v * v); }
bool Vector4::operator > (int v) const                  { return MagnitudeSquared() > (float)(v * v); }
bool Vector4::operator > (const class Vector2& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector4::operator > (const class Vector3& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector4::operator >= (const Vector4& v) const      { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector4::operator >= (float v) const               { return MagnitudeSquared() >= (v * v); }
bool Vector4::operator >= (double v) const              { return MagnitudeSquared() >= (float)(v * v); }
bool Vector4::operator >= (int v) const                 { return MagnitudeSquared() >= (float)(v * v); }
bool Vector4::operator >= (const class Vector2& v) const      { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector4::operator >= (const class Vector3& v) const      { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector4::operator == (const Vector4& v) const {
#   if defined(XO_SSE2)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.xmm, xmm)), sse::Epsilon)) & 15) == 15;
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

bool Vector4::operator == (const class Vector2& v) const {
#   if defined(XO_SSE)
    // Todo: check that this is actually faster.
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(xmm, _mm_set_ps(0.0f, 0.0f, v.y, v.x))), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}

bool Vector4::operator == (const class Vector3& v) const {
#   if defined(XO_SSE)
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.xmm, xmm)), sse::Epsilon)) & 7) == 7;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon) && CloseEnough(z, v.z, Epsilon);
#   endif
}

bool Vector4::operator != (const Vector4& v) const  { return !((*this) == v); }
bool Vector4::operator != (float v) const           { return !((*this) == v); }
bool Vector4::operator != (double v) const          { return !((*this) == v); }
bool Vector4::operator != (int v) const             { return !((*this) == v); }
bool Vector4::operator != (const class Vector2& v) const  { return !((*this) == v); }
bool Vector4::operator != (const class Vector3& v) const  { return !((*this) == v); }

_XOINL 
Vector4 Abs(const Vector4& v)
{
#if defined(XO_SSE)
    return (sse::Abs(v.xmm));
#else
    return Vector4(Abs(v.x), Abs(v.y), Abs(v.z));
#endif
}

#undef IDX_X
#undef IDX_Y
#undef IDX_Z
#undef IDX_W

XOMATH_END_XO_NS();