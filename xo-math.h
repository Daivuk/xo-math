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

#ifndef XO_MATH_H
#define XO_MATH_H

#include "xo-math-config.h"

////////////////////////////////////////////////////////////////////////// Optional defines for configuration
#ifdef XO_CUSTOM_NS
#   define XOMATH_BEGIN_XO_NS()  namespace XO_CUSTOM_NS {
#   define XOMATH_END_XO_NS()    }
#elif defined(XO_SINGLE_NS)
#   define XOMATH_BEGIN_XO_NS()  namespace xo {
#   define XOMATH_END_XO_NS()    }
#elif defined(XO_SIMPLE_NS)
#   define XOMATH_BEGIN_XO_NS()  namespace math {
#   define XOMATH_END_XO_NS()    }
#elif defined(XO_NO_NS)
#   define XOMATH_BEGIN_XO_NS()
#   define XOMATH_END_XO_NS()
#else
#   define XOMATH_BEGIN_XO_NS()  namespace xo { namespace math {
#   define XOMATH_END_XO_NS()    } }
#endif

#if !defined(XO_ASSERT)
#   define XO_ASSERT(condition, message)
#endif

#if defined(XO_SPACE_LEFTHAND) && defined(XO_SPACE_RIGHTHAND)
static_assert(false, "xo-math found both XO_SPACE_LEFTHAND and XO_SPACE_RIGHTHAND defined. These are incompatible");
#elif !defined(XO_SPACE_LEFTHAND) && !defined(XO_SPACE_RIGHTHAND)
    //! Default to right hand space if no configuration is defined.
    //! @sa https://www.evl.uic.edu/ralph/508S98/coordinates.html
#   define XO_SPACE_RIGHTHAND 1
#endif

#if defined(XO_SPACE_YUP) && defined(XO_SPACE_ZUP)
static_assert(false, "xo-math found both XO_SPACE_YUP and XO_SPACE_ZUP defined. These are incompatible");
#elif !defined(XO_SPACE_YUP) && !defined(XO_SPACE_ZUP)
    //! Default to z up if no configuration is defined.
#   define XO_SPACE_ZUP
#endif

////////////////////////////////////////////////////////////////////////// Dependencies for xo-math headers
#include <math.h>
#include <ostream>
#include <random>
#include <thread>
#include <limits>
#if defined(_MSC_VER)
#   include <xmmintrin.h>
#else
#   include <x86intrin.h>
#endif

// Not available in clang, so far as I can tell.
#ifndef _MM_ALIGN16
#   define _MM_ALIGN16 __attribute__((aligned(16)))
#endif

#if defined(_XOMATH_INTERNAL_MACRO_WARNING)
static_assert(false, "xo-math found an internal macro where it shouldn't have.");
#else
#   define _XOMATH_INTERNAL_MACRO_WARNING static_assert(false, "xo-math found an internal macro where it shouldn't have.");
#endif

#if defined(XOMATH_INTERNAL)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   define XOMATH_INTERNAL 1
#endif

#if !defined(_XO_MATH_STRINGIFY_HELPER)
    // Do not undef at end of file. Will break XO_MATH_VERSION... and XO_MATH_COMPILER... defines
#   define _XO_MATH_STRINGIFY_HELPER(x) #x
#endif

#if !defined(_XO_MATH_STRINGIFY)
    // Do not undef at end of file. Will break XO_MATH_VERSION... and XO_MATH_COMPILER... defines
#   define _XO_MATH_STRINGIFY(x) _XO_MATH_STRINGIFY_HELPER(x)
#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include DetectSIMD.h directly. Include xo-math.h.");
#else // XOMATH_INTERNAL

#if !defined(XO_SSE) && !defined(XO_SSE2) && !defined(XO_SSE3) && !defined(XO_SSSE3) && !defined(XO_SSE4_1) && !defined(XO_SSE4_2) && !defined(XO_AVX) && !defined(XO_AVX2) && !defined(XO_AVX512)
#   if defined(_MSC_VER)
#       if defined(_M_IX86_FP)
#           if _M_IX86_FP == 1
#               define XO_SSE 1
#           elif _M_IX86_FP == 2
#               define XO_SSE 1
#               define XO_SSE2 1
#           endif
#       endif
#       if defined(__AVX__)
#           define XO_SSE 1
#           define XO_SSE2 1
#           define XO_SSE3 1
#           define XO_SSSE3 1
#           define XO_SSE4_1 1
#           define XO_SSE4_2 1
#           define XO_AVX 1
#       endif
#       if defined(__AVX2__)
#           define XO_SSE 1
#           define XO_SSE2 1
#           define XO_SSE3 1
#           define XO_SSSE3 1
#           define XO_SSE4_1 1
#           define XO_SSE4_2 1
#           define XO_AVX 1
#           define XO_AVX2 1
#       endif
#   elif defined(__clang__) || defined (__GNUC__)
#       if defined(__SSE__)
#           define XO_SSE 1
#       endif
#       if defined(__SSE2__)
#           define XO_SSE2 1
#       endif
#       if defined(__SSE3__)
#           define XO_SSE3 1
#       endif
#       if defined(__SSSE3__)
#           define XO_SSSE3 1
#       endif
#       if defined(__SSE4_1__)
#           define XO_SSE4_1 1
#       endif
#       if defined(__SSE4_2__)
#           define XO_SSE4_2 1
#       endif
#       if defined(__AVX__)
#           define XO_AVX 1
#       endif
#       if defined(__AVX2__)
#           define XO_AVX2 1
#       endif
#       if defined(__AVX512__) || defined(__AVX512F__)
#           define XO_AVX512 1
#       endif
#   endif
// If the user really knows what they're doing, they should be able to override seriously any of these defines they want.
// If XO_NO_VECTOR_DETECT_WARNING is not defined then we'll tell them about the lack of support on their platform 
#elif !defined(XO_NO_VECTOR_DETECT_WARNING)
#   if defined(_MSC_VER)
#       if defined(_XO_MATH_MSC_PRAGMALINK)
_XOMATH_INTERNAL_MACRO_WARNING
#       else
#           define _XO_MATH_MSC_PRAGMALINK __FILE__ "(" _XO_MATH_STRINGIFY(__LINE__) ") : "
#       endif
#       if defined(_XO_MATH_WARN)
_XOMATH_INTERNAL_MACRO_WARNING
#       else
#           define _XO_MATH_WARN(exp) (_XO_MATH_MSC_PRAGMALINK "WARNING: " exp)
#       endif

#       if !(defined(__AVX__) || defined(__AVX2__) || defined(_M_IX86_FP) || _M_IX86_FP < 1) && defined(XO_SSE)
#           undef XO_SSE
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__) || defined(_M_IX86_FP) || _M_IX86_FP < 2) && defined(XO_SSE2)
#           undef XO_SSE2
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE3)
#           undef XO_SSE3
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSSE3)
#           undef XO_SSSE3
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE4_1)
#           undef XO_SSE4_1
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE4_1 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_SSE4_2)
#           undef XO_SSE4_2
#           pragma message _XO_MATH_WARN("xo-math detected that XO_SSE4_2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !(defined(__AVX__) || defined(__AVX2__)) && defined(XO_AVX)
#           undef XO_AVX
#           pragma message _XO_MATH_WARN("xo-math detected that XO_AVX is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
#       if !defined(__AVX2__) && defined(XO_AVX2)
#           undef XO_AVX2
#           pragma message _XO_MATH_WARN("xo-math detected that XO_AVX2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif
        // Hey, if you know for a fact that AVX512 is supported on your version of msvc
        // could you please find out if there's a macro to detect it? (such as __AVX512__?)
        // At the time of writing, there is no such macro documented.
        // If there is I'd appreciate a quick email or pull request. I can be reached at jared@xoorath.com
        
        // Assuming __AVX512__ is the macro for example, you can substitute:

//#     if defined(XO_AVX512)
        // for
//#     !defined(__AVX512__) && defined(XO_AVX512)

        // to get your code to stop throwing this warning.
#       if defined(XO_AVX512)
#           undef XO_AVX512
#           pragma message _XO_MATH_WARN("xo-math detected that XO_AVX512 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features.")
#       endif

#   undef _XO_MATH_MSC_PRAGMALINK
#   undef _XO_MATH_WARN
#   elif defined(__clang__) || defined (__GNUC__)
#       if !defined(__SSE__) && defined(XO_SSE)
#           undef XO_SSE
#           warning "xo-math detected that XO_SSE is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE2__) && defined(XO_SSE2)
#           undef XO_SSE2
#           warning "xo-math detected that XO_SSE2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE3__) && defined(XO_SSE3)
#           undef XO_SSE3
#           warning "xo-math detected that XO_SSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSSE3__) && defined(XO_SSSE3)
#           undef XO_SSSE3
#           warning "xo-math detected that XO_SSSE3 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE4_1__) && defined(XO_SSE4_1)
#           undef XO_SSE4_1
#           warning "xo-math detected that XO_SSE4_1 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__SSE4_2__) && defined(XO_SSE4_2)
#           undef XO_SSE4_2
#           warning "xo-math detected that XO_SSE4_2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX__) && defined(XO_AVX)
#           undef XO_AVX
#           warning "xo-math detected that XO_AVX is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX2__) && defined(XO_AVX2)
#           undef XO_AVX2
#           warning "xo-math detected that XO_AVX2 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#       if !defined(__AVX512__) && defined(XO_AVX512)
#           undef XO_AVX512
#           warning "xo-math detected that XO_AVX512 is defined when the compiler doesn't have this feature enabled. We're going to undefine it for you to prevent compilation failure. Please see your compiler documentation to enable various SIMD features."
#       endif
#   endif
#endif

#if defined(XO_AVX512)
#   define XO_MATH_HIGHEST_SIMD "avx512"
#elif defined(XO_AVX2)
#   define XO_MATH_HIGHEST_SIMD "avx2"
#elif defined(XO_AVX)
#   define XO_MATH_HIGHEST_SIMD "avx"
#elif defined(XO_SSE4_2)
#   define XO_MATH_HIGHEST_SIMD "sse4.2"
#elif defined(XO_SSE4_1)
#   define XO_MATH_HIGHEST_SIMD "sse4.1"
#elif defined(XO_SSSE3)
#   define XO_MATH_HIGHEST_SIMD "ssse3"
#elif defined(XO_SSE3)
#   define XO_MATH_HIGHEST_SIMD "sse3"
#elif defined(XO_SSE2)
#   define XO_MATH_HIGHEST_SIMD "sse2"
#elif defined(XO_SSE)
#   define XO_MATH_HIGHEST_SIMD "sse"
#else
#   define XO_MATH_HIGHEST_SIMD "none"
#endif

#endif // XOMATH_INTERNAL


#if defined(_XOCONSTEXPR) || defined(_XONOCONSTEXPR)
_XOMATH_INTERNAL_MACRO_WARNING
#else
// todo: remove constexpr in visual studio 2013 and re-test for support
#   if defined(_MSC_VER) && _MSC_VER < 1800
#       define _XOCONSTEXPR
#       define _XONOCONSTEXPR
#    else
#       define _XOCONSTEXPR constexpr
#    endif
#endif

#if defined(_XOINL)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   ifdef XO_NO_INLINE
        // I've found at least on msvc that inline will improve execution time in our test main. on 100k iterations I saw 0.51s without inline and 0.46s with inline.
#       define _XOINL
#   elif defined(_MSC_VER)
#       define _XOINL __forceinline
#   else
#       define _XOINL inline
#   endif
#endif

#if defined(_XOTLS)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   if (defined(__clang__) && defined(__APPLE__))
#       define _XOTLS __thread
#	elif (defined(_MSC_VER) && _MSC_VER < 1800)
#       define _XOTLS __declspec(thread)
#   else
#       define _XOTLS thread_local
#   endif
#endif

#if defined(_XO_TLS_ENGINE)
_XOMATH_INTERNAL_MACRO_WARNING
#else
    // apple clang doesn't give us thread_local until xcode 8.
#   if (defined(__clang__) && defined(__APPLE__)) || (defined(_MSC_VER) && _MSC_VER < 1800)
#       define _XO_TLS_ENGINE \
            static _XOTLS std::mt19937* tls_engline; \
            static _XOTLS char mem[sizeof(std::mt19937)];\
            if(!tls_engline) { \
                tls_engline = new(mem) std::mt19937((unsigned)clock()); \
            }
#   else
#       define _XO_TLS_ENGINE \
            static _XOTLS std::mt19937 tls_engline((unsigned)clock());
#   endif
#endif

#if defined(_XO_TLS_DISTRIBUTION)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   if defined(__clang__) && defined(__APPLE__) || (defined(_MSC_VER) && _MSC_VER < 1800)
#       define _XO_TLS_DISTRIBUTION dist(*tls_engline)
#   else
#       define _XO_TLS_DISTRIBUTION dist(tls_engline)
#   endif
#endif

XOMATH_BEGIN_XO_NS();

_XOCONSTEXPR const float PI = 3.141592653589793238462643383279502884197169399375105820f;
_XOCONSTEXPR const float PIx2 = 2.0f * PI;
_XOCONSTEXPR const float TAU = PIx2;
_XOCONSTEXPR const float HalfPI = PI / 2.0f;

_XOCONSTEXPR const float FloatEpsilon = 0.0000001192092896f;

_XOCONSTEXPR const float Rad2Deg = 360.0f / TAU;
_XOCONSTEXPR const float Deg2Rad = TAU / 360.0f;

_XOINL float HexFloat(unsigned u) {
    union {
        unsigned u;
        float f;
    } Converter;
    Converter.u = u;
    return Converter.f;
}

#if XO_SSE
namespace sse {
    static const __m128 AbsMask = _mm_set1_ps(HexFloat(0x7fffffff));

    _XOINL __m128 Abs(__m128 v) {
        return _mm_and_ps(AbsMask, v);
    }

    // the quoted error on _mm_rcp_ps documentation
    _XOCONSTEXPR const float SSEFloatEpsilon = 0.000366210938f;

    static const __m128 Zero = _mm_setzero_ps();
    static const __m128 One = _mm_set1_ps(1.0f);
    static const __m128 NegativeOne = _mm_set1_ps(-1.0f);
    static const __m128 Epsilon = _mm_set_ps1(SSEFloatEpsilon);
}

// We wont warn about pre-defining XO_16ALIGNED_MALLOC or XO_16ALIGNED_FREE.
// If a user wants to create their own allocator, we wont get in the way of that.
#   if !defined(XO_16ALIGNED_MALLOC)
#       define XO_16ALIGNED_MALLOC(size) _mm_malloc(size, 16)
#   endif
#   if  !defined(XO_16ALIGNED_FREE)
#       define XO_16ALIGNED_FREE(ptr) _mm_free(ptr)
#   endif

#   if defined(_XO_OVERLOAD_NEW_DELETE)
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#   define _XO_OVERLOAD_NEW_DELETE() \
        _XOINL static void* operator new (std::size_t size)     { return XO_16ALIGNED_MALLOC(size); } \
        _XOINL static void* operator new[] (std::size_t size)   { return XO_16ALIGNED_MALLOC(size); } \
        _XOINL static void operator delete (void* ptr)          { XO_16ALIGNED_FREE(ptr); } \
        _XOINL static void operator delete[] (void* ptr)        { XO_16ALIGNED_FREE(ptr); }
#   endif

#else
// we don't need to overload new and delete unless memory alignment is required.
#if defined(_XO_OVERLOAD_NEW_DELETE)
_XOMATH_INTERNAL_MACRO_WARNING
#   else
#       define _XO_OVERLOAD_NEW_DELETE()
#   endif
#endif

#if defined(_XO_MIN)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   define _XO_MIN(a, b) (a < b ? a : b)
#endif

#if defined(_XO_MAX)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   define _XO_MAX(a, b) (a > b ? a : b)
#endif

// wrap for now, so we have the option to make a faster version later.
_XOINL float Min(float x, float y)      { return _XO_MIN(x, y); }
_XOINL float Max(float x, float y)      { return _XO_MAX(x, y); }
_XOINL float Abs(float f)               { return f > 0.0f ? f : -f; }
_XOINL float Sqrt(float f)              { return sqrtf(f); }
_XOINL float Sin(float f)               { return sinf(f); }
_XOINL float Cos(float f)               { return cosf(f); }
_XOINL float Tan(float f)               { return tanf(f); }
_XOINL float ASin(float f)              { return asinf(f); }
_XOINL float ACos(float f)              { return acosf(f); }
_XOINL float ATan(float f)              { return atanf(f); }
_XOINL float ATan2(float y, float x)    { return atan2f(y, x); }
_XOINL float Difference(float x, float y) { return Abs(x-y); }
_XOINL
bool CloseEnough(float x, float y, float tolerance = FloatEpsilon) {
    return Difference(x, y) * (1.0f/tolerance) <= Min(Abs(x), Abs(y));
}

_XOCONSTEXPR _XOINL float Square(float t)      { return t*t; }
_XOCONSTEXPR _XOINL double Square(double t)    { return t*t; }
_XOCONSTEXPR _XOINL int Square(int t)          { return t*t; }

_XOINL 
bool RandomBool() {
    _XO_TLS_ENGINE
    std::uniform_int_distribution<int> dist(0, 1);
    return _XO_TLS_DISTRIBUTION == 1;
}

_XOINL 
int RandomRange(int low, int high) {
    _XO_TLS_ENGINE
    std::uniform_int_distribution<int> dist(low, high);
    return _XO_TLS_DISTRIBUTION;
}

_XOINL 
float RandomRange(float low, float high) {
    _XO_TLS_ENGINE
    std::uniform_real_distribution<float> dist(low, high);
    return _XO_TLS_DISTRIBUTION;
}

XOMATH_END_XO_NS();

#if defined(_XO_ASSIGN_QUAT)
_XOMATH_INTERNAL_MACRO_WARNING
#   undef _XO_ASSIGN_QUAT
#endif
#if defined(_XO_ASSIGN_QUAT_Q)
_XOMATH_INTERNAL_MACRO_WARNING
#   undef _XO_ASSIGN_QUAT_Q
#endif

#if XO_SSE
#define _XO_ASSIGN_QUAT(W, X, Y, Z) m = _mm_set_ps(W, Z, Y, X);
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) Q.m = _mm_set_ps(W, Z, Y, X);
#else
#define _XO_ASSIGN_QUAT(W, X, Y, Z) this->w = W; this->x = X; this->y = Y; this->z = Z;
#define _XO_ASSIGN_QUAT_Q(Q, W, X, Y, Z) Q.w = W; Q.x = X; Q.y = Y; Q.z = Z;
#endif

////////////////////////////////////////////////////////////////////////// Module Includes
#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector2.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Vector2 {
public:

    ////////////////////////////////////////////////////////////////////////// Constructors
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#constructors
    _XOINL Vector2(); 
    _XOINL Vector2(float v); 
    _XOINL Vector2(float x, float y); 
    _XOINL Vector2(const Vector2& v); 
    _XOINL Vector2(const class Vector3& v); 
    _XOINL Vector2(const class Vector4& v); 

    ////////////////////////////////////////////////////////////////////////// Set / Get Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#set_get_methods
    _XOINL const Vector2& Set(float x, float y);
    _XOINL const Vector2& Set(float f);
    _XOINL const Vector2& Set(const Vector2& vec);
    _XOINL void Get(float& x, float& y) const;
    _XOINL void Get(float* f) const;

    ////////////////////////////////////////////////////////////////////////// Special Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#special_operators
    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;
    _XOINL Vector2 operator - () const;
    _XOINL Vector2 operator ~ () const;


    ////////////////////////////////////////////////////////////////////////// Math Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#math_operators
    _XOINL const Vector2& operator += (const Vector2& v);
    _XOINL const Vector2& operator += (float v);
    _XOINL const Vector2& operator += (double v);
    _XOINL const Vector2& operator += (int v);
    _XOINL const Vector2& operator += (const class Vector3& v);
    _XOINL const Vector2& operator += (const class Vector4& v);
    _XOINL const Vector2& operator -= (const Vector2& v);
    _XOINL const Vector2& operator -= (float v);
    _XOINL const Vector2& operator -= (double v);
    _XOINL const Vector2& operator -= (int v);
    _XOINL const Vector2& operator -= (const class Vector3& v);
    _XOINL const Vector2& operator -= (const class Vector4& v);
    _XOINL const Vector2& operator *= (const Vector2& v);
    _XOINL const Vector2& operator *= (float v);
    _XOINL const Vector2& operator *= (double v);
    _XOINL const Vector2& operator *= (int v);
    _XOINL const Vector2& operator *= (const class Vector3& v);
    _XOINL const Vector2& operator *= (const class Vector4& v);
    _XOINL const Vector2& operator /= (const Vector2& v);
    _XOINL const Vector2& operator /= (float v);
    _XOINL const Vector2& operator /= (double v);
    _XOINL const Vector2& operator /= (int v);
    _XOINL const Vector2& operator /= (const class Vector3& v);
    _XOINL const Vector2& operator /= (const class Vector4& v);
    _XOINL Vector2 operator + (const Vector2& v) const;
    _XOINL Vector2 operator + (float v) const;
    _XOINL Vector2 operator + (double v) const;
    _XOINL Vector2 operator + (int v) const;
    _XOINL Vector2 operator + (const class Vector3& v) const;
    _XOINL Vector2 operator + (const class Vector4& v) const;
    _XOINL Vector2 operator - (const Vector2& v) const;
    _XOINL Vector2 operator - (float v) const;
    _XOINL Vector2 operator - (double v) const;
    _XOINL Vector2 operator - (int v) const;
    _XOINL Vector2 operator - (const class Vector3& v) const;
    _XOINL Vector2 operator - (const class Vector4& v) const;
    _XOINL Vector2 operator * (const Vector2& v) const;
    _XOINL Vector2 operator * (float v) const;
    _XOINL Vector2 operator * (double v) const;
    _XOINL Vector2 operator * (int v) const;
    _XOINL Vector2 operator * (const class Vector3& v) const;
    _XOINL Vector2 operator * (const class Vector4& v) const;
    _XOINL Vector2 operator / (const Vector2& v) const;
    _XOINL Vector2 operator / (float v) const;
    _XOINL Vector2 operator / (double v) const;
    _XOINL Vector2 operator / (int v) const;
    _XOINL Vector2 operator / (const class Vector3& v) const;
    _XOINL Vector2 operator / (const class Vector4& v) const;

    ////////////////////////////////////////////////////////////////////////// Comparison Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#comparison_operators
    _XOINL bool operator < (const Vector2& v) const;
    _XOINL bool operator < (float v) const;
    _XOINL bool operator < (double v) const;
    _XOINL bool operator < (int v) const;
    _XOINL bool operator < (const class Vector3& v) const;
    _XOINL bool operator < (const class Vector4& v) const;
    _XOINL bool operator <= (const Vector2& v) const;
    _XOINL bool operator <= (float v) const;
    _XOINL bool operator <= (double v) const;
    _XOINL bool operator <= (int v) const;
    _XOINL bool operator <= (const class Vector3& v) const;
    _XOINL bool operator <= (const class Vector4& v) const;
    _XOINL bool operator > (const Vector2& v) const;
    _XOINL bool operator > (float v) const;
    _XOINL bool operator > (double v) const;
    _XOINL bool operator > (int v) const;
    _XOINL bool operator > (const class Vector3& v) const;
    _XOINL bool operator > (const class Vector4& v) const;
    _XOINL bool operator >= (const Vector2& v) const;
    _XOINL bool operator >= (float v) const;
    _XOINL bool operator >= (double v) const;
    _XOINL bool operator >= (int v) const;
    _XOINL bool operator >= (const class Vector3& v) const;
    _XOINL bool operator >= (const class Vector4& v) const;

    _XOINL bool operator == (const Vector2& v) const;
    _XOINL bool operator == (float v) const;
    _XOINL bool operator == (double v) const;
    _XOINL bool operator == (int v) const;
    _XOINL bool operator == (const class Vector3& v) const;
    _XOINL bool operator == (const class Vector4& v) const;
    _XOINL bool operator != (const Vector2& v) const;
    _XOINL bool operator != (float v) const;
    _XOINL bool operator != (double v) const;
    _XOINL bool operator != (int v) const;
    _XOINL bool operator != (const class Vector3& v) const;
    _XOINL bool operator != (const class Vector4& v) const;

    ////////////////////////////////////////////////////////////////////////// Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#methods
    _XOINL bool IsNormalized() const;
    _XOINL bool IsZero() const;
    _XOINL float Magnitude() const;
    _XOINL float MagnitudeSquared() const;
    _XOINL float Sum() const;

    _XOINL const Vector2& Normalize();

    _XOINL Vector2 Normalized() const;

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#static_methods
    _XOINL static void Lerp(const Vector2& a, const Vector2& b, float t, Vector2& outVec);
    _XOINL static void Max(const Vector2& a, const Vector2& b, Vector2& outVec);
    _XOINL static void Midpoint(const Vector2& a, const Vector2& b, Vector2& outVec);
    _XOINL static void Min(const Vector2& a, const Vector2& b, Vector2& outVec);
    _XOINL static void OrthogonalCCW(const Vector2& v, Vector2& outVec);
    _XOINL static void OrthogonalCW(const Vector2& v, Vector2& outVec);

    _XOINL static float AngleDegrees(const Vector2& a, const Vector2& b);
    _XOINL static float AngleRadians(const Vector2& a, const Vector2& b);
    _XOINL static float Cross(const Vector2& a, const Vector2& b);
    _XOINL static float Distance(const Vector2& a, const Vector2& b);
    _XOINL static float DistanceSquared(const Vector2& a, const Vector2& b);
    _XOINL static float Dot(const Vector2& a, const Vector2& b);

    ////////////////////////////////////////////////////////////////////////// Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#variants
    _XOINL static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);
    _XOINL static Vector2 Max(const Vector2& a, const Vector2& b);
    _XOINL static Vector2 Midpoint(const Vector2& a, const Vector2& b);
    _XOINL static Vector2 Min(const Vector2& a, const Vector2& b);
    _XOINL static Vector2 OrthogonalCCW(const Vector2& v);
    _XOINL static Vector2 OrthogonalCW(const Vector2& v);

    _XOINL float AngleDegrees(const Vector2& v) const;
    _XOINL float AngleRadians(const Vector2& v) const;
    _XOINL float Cross(const Vector2& v) const;
    _XOINL float Distance(const Vector2& v) const;
    _XOINL float DistanceSquared(const Vector2& v) const;
    _XOINL float Dot(const Vector2& v) const;
    _XOINL Vector2 Lerp(const Vector2& v, float t) const;
    _XOINL Vector2 Midpoint(const Vector2& v) const;
    _XOINL Vector2 OrthogonalCCW() const;
    _XOINL Vector2 OrthogonalCW() const;

    ////////////////////////////////////////////////////////////////////////// Extras
    // See: http://xo-math.rtfd.io/en/latest/classes/vector2.html#extras
    friend std::ostream& operator <<(std::ostream& os, const Vector2& v) {
        os << "(x:" << v.x << ", y:" << v.y << ", mag:" << v.Magnitude() << ")";
        return os;
    }

    static const Vector2
        UnitX, 
        UnitY,
        Up,
        Down,
        Left,
        Right,
        One,
        Zero;

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 2.0f;
#endif

    union {
        struct { float x, y; };
        float f[2];
    };
};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL


#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Vector3 {
public:
    ////////////////////////////////////////////////////////////////////////// Constructors
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#constructors
    _XOINL Vector3(); 
    _XOINL Vector3(float f); 
    _XOINL Vector3(float x, float y, float z); 
    _XOINL Vector3(const Vector3& vec); 
#if XO_SSE
    _XOINL Vector3(const __m128& vec); 
#endif
    _XOINL Vector3(const class Vector2& v); 
    _XOINL Vector3(const class Vector4& v); 

    ////////////////////////////////////////////////////////////////////////// Set / Get Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#set_get_methods
    _XOINL const Vector3& Set(float x, float y, float z);
    _XOINL const Vector3& Set(float f);
    _XOINL const Vector3& Set(const Vector3& vec);
#if XO_SSE
    _XOINL const Vector3& Set(const __m128& vec);
#endif
    _XOINL void Get(float& x, float& y, float &z) const;
    _XOINL void Get(float* f) const;

    ////////////////////////////////////////////////////////////////////////// Special Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#special_operators
    _XO_OVERLOAD_NEW_DELETE();
#if XO_SSE
    _XOINL operator __m128() const;
#endif
    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;
    _XOINL Vector3 operator -() const;
    _XOINL Vector3 operator ~() const;

    ////////////////////////////////////////////////////////////////////////// Math Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#math_operators
    _XOINL const Vector3& operator += (const Vector3& v);
    _XOINL const Vector3& operator += (float v);
    _XOINL const Vector3& operator += (double v);
    _XOINL const Vector3& operator += (int v);
    _XOINL const Vector3& operator += (const class Vector2& v);
    _XOINL const Vector3& operator += (const class Vector4& v);
    _XOINL const Vector3& operator -= (const Vector3& v);
    _XOINL const Vector3& operator -= (float v);
    _XOINL const Vector3& operator -= (double v);
    _XOINL const Vector3& operator -= (int v);
    _XOINL const Vector3& operator -= (const class Vector2& v);
    _XOINL const Vector3& operator -= (const class Vector4& v);
    _XOINL const Vector3& operator *= (const Vector3& v);
    _XOINL const Vector3& operator *= (float v);
    _XOINL const Vector3& operator *= (double v);
    _XOINL const Vector3& operator *= (int v);
    _XOINL const Vector3& operator *= (const class Vector2& v);
    _XOINL const Vector3& operator *= (const class Vector4& v);
    _XOINL const Vector3& operator /= (const Vector3& v);
    _XOINL const Vector3& operator /= (float v);
    _XOINL const Vector3& operator /= (double v);
    _XOINL const Vector3& operator /= (int v);
    _XOINL const Vector3& operator /= (const class Vector2& v);
    _XOINL const Vector3& operator /= (const class Vector4& v);
    _XOINL Vector3 operator + (const Vector3& v) const;
    _XOINL Vector3 operator + (float v) const;
    _XOINL Vector3 operator + (double v) const;
    _XOINL Vector3 operator + (int v) const;
    _XOINL Vector3 operator + (const class Vector2& v) const;
    _XOINL Vector3 operator + (const class Vector4& v) const;
    _XOINL Vector3 operator - (const Vector3& v) const;
    _XOINL Vector3 operator - (float v) const;
    _XOINL Vector3 operator - (double v) const;
    _XOINL Vector3 operator - (int v) const;
    _XOINL Vector3 operator - (const class Vector2& v) const;
    _XOINL Vector3 operator - (const class Vector4& v) const;
    _XOINL Vector3 operator * (const Vector3& v) const;
    _XOINL Vector3 operator * (float v) const;
    _XOINL Vector3 operator * (double v) const;
    _XOINL Vector3 operator * (int v) const;
    _XOINL Vector3 operator * (const class Vector2& v) const;
    _XOINL Vector3 operator * (const class Vector4& v) const;
    _XOINL Vector3 operator / (const Vector3& v) const;
    _XOINL Vector3 operator / (float v) const;
    _XOINL Vector3 operator / (double v) const;
    _XOINL Vector3 operator / (int v) const;
    _XOINL Vector3 operator / (const class Vector2& v) const;
    _XOINL Vector3 operator / (const class Vector4& v) const;

    ////////////////////////////////////////////////////////////////////////// Comparison Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#comparison_operators
    _XOINL bool operator < (const Vector3& v) const;
    _XOINL bool operator < (float v) const;
    _XOINL bool operator < (double v) const;
    _XOINL bool operator < (int v) const;
    _XOINL bool operator < (const class Vector2& v) const;
    _XOINL bool operator < (const class Vector4& v) const;
    _XOINL bool operator <= (const Vector3& v) const;
    _XOINL bool operator <= (float v) const;
    _XOINL bool operator <= (double v) const;
    _XOINL bool operator <= (int v) const;
    _XOINL bool operator <= (const class Vector2& v) const;
    _XOINL bool operator <= (const class Vector4& v) const;
    _XOINL bool operator > (const Vector3& v) const;
    _XOINL bool operator > (float v) const;
    _XOINL bool operator > (double v) const;
    _XOINL bool operator > (int v) const;
    _XOINL bool operator > (const class Vector2& v) const;
    _XOINL bool operator > (const class Vector4& v) const;
    _XOINL bool operator >= (const Vector3& v) const;
    _XOINL bool operator >= (float v) const;
    _XOINL bool operator >= (double v) const;
    _XOINL bool operator >= (int v) const;
    _XOINL bool operator >= (const class Vector2& v) const;
    _XOINL bool operator >= (const class Vector4& v) const;

    _XOINL bool operator == (const Vector3& v) const;
    _XOINL bool operator == (float v) const;
    _XOINL bool operator == (double v) const;
    _XOINL bool operator == (int v) const;
    _XOINL bool operator == (const class Vector2& v) const;
    _XOINL bool operator == (const class Vector4& v) const;
    _XOINL bool operator != (const Vector3& v) const;
    _XOINL bool operator != (float v) const;
    _XOINL bool operator != (double v) const;
    _XOINL bool operator != (int v) const;
    _XOINL bool operator != (const class Vector2& v) const;
    _XOINL bool operator != (const class Vector4& v) const;

    ////////////////////////////////////////////////////////////////////////// Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#methods
    _XOINL bool IsNormalized() const;
    _XOINL bool IsZero() const;
    _XOINL float Magnitude() const;
    _XOINL float MagnitudeSquared() const;
    _XOINL float Sum() const;

    _XOINL const Vector3& Normalize();

    _XOINL Vector3 Normalized() const;
    _XOINL Vector3 ZYX() const;
    

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#static_methods
    _XOINL static void Cross(const Vector3& a, const Vector3& b, Vector3& outVec);
    _XOINL static void Lerp(const Vector3& a, const Vector3& b, float t, Vector3& outVec);
    _XOINL static void Max(const Vector3& a, const Vector3& b, Vector3& outVec);
    _XOINL static void Min(const Vector3& a, const Vector3& b, Vector3& outVec);
    _XOINL static void RandomAtDistance(float d, Vector3& outVec);
    _XOINL static void RandomInConeDegrees(const Vector3& forward, float angle, Vector3& outVec);
    _XOINL static void RandomInConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    _XOINL static void RandomInDistance(float d, Vector3& outVec);
    _XOINL static void RandomInRange(float low, float high, Vector3& outVec);
    _XOINL static void RandomInSphere(Vector3& outVec);
    _XOINL static void RandomOnConeDegrees(const Vector3& forward, float angle, Vector3& outVec);
    _XOINL static void RandomOnConeRadians(const Vector3& forward, float angle, Vector3& outVec);
    _XOINL static void RandomOnSphere(Vector3& outVec);
    _XOINL static void RotateDegrees(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec);
    _XOINL static void RotateRadians(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec);
    // Calls Vector3::AngleRadians, converting the return value to degrees.
    _XOINL static float AngleDegrees(const Vector3& a, const Vector3& b);
    _XOINL static float AngleRadians(const Vector3& a, const Vector3& b);
    _XOINL static float Distance(const Vector3&a, const Vector3&b);
    _XOINL static float DistanceSquared(const Vector3& a, const Vector3& b);
    _XOINL static float Dot(const Vector3& a, const Vector3& b);
    

    ////////////////////////////////////////////////////////////////////////// Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#variants
    _XOINL static Vector3 Cross(const Vector3& a, const Vector3& b);
    _XOINL static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
    _XOINL static Vector3 Max(const Vector3& a, const Vector3& b);
    _XOINL static Vector3 Min(const Vector3& a, const Vector3& b);
    _XOINL static Vector3 RandomAtDistance(float d);
    _XOINL static Vector3 RandomInConeDegrees(const Vector3& forward, float angle);
    _XOINL static Vector3 RandomInConeRadians(const Vector3& forward, float angle);
    _XOINL static Vector3 RandomInDistance(float d);
    _XOINL static Vector3 RandomInRange(float low, float high);
    _XOINL static Vector3 RandomInSphere();
    _XOINL static Vector3 RandomOnConeDegrees(const Vector3& forward, float angle);
    _XOINL static Vector3 RandomOnConeRadians(const Vector3& forward, float angle);
    _XOINL static Vector3 RandomOnSphere();
    _XOINL static Vector3 RotateDegrees(const Vector3& v, const Vector3& axis, float angle);
    _XOINL static Vector3 RotateRadians(const Vector3& v, const Vector3& axis, float angle);

    _XOINL float AngleDegrees(const Vector3& v) const;
    _XOINL float AngleRadians(const Vector3& v) const;
    _XOINL float Distance(const Vector3& v) const;
    _XOINL float DistanceSquared(const Vector3& v) const;
    _XOINL float Dot(const Vector3& v) const;
    _XOINL Vector3 Cross(const Vector3& v) const;
    _XOINL Vector3 Lerp(const Vector3& v, float t) const;
    _XOINL Vector3 RandomInConeDegrees(float angle) const;
    _XOINL Vector3 RandomInConeRadians(float angle) const;
    _XOINL Vector3 RandomOnConeDegrees(float angle) const;
    _XOINL Vector3 RandomOnConeRadians(float angle) const;
    _XOINL Vector3 RotateDegrees(const Vector3& axis, float angle) const;
    _XOINL Vector3 RotateRadians(const Vector3& axis, float angle) const;

    ////////////////////////////////////////////////////////////////////////// Extras
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#extras
    friend std::ostream& operator <<(std::ostream& os, const Vector3& v) {
#if XO_SSE
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
#else
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", mag:" << v.Magnitude() << ")";
#endif
        return os;
    }

    ////////////////////////////////////////////////////////////////////////// Static Attributes
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#public_static_attributes
    static const Vector3
        Origin, 
        UnitX, 
        UnitY, 
        UnitZ, 
        Up,
        Down,
        Left, 
        Right, 
        Forward,
        Backward,
        One, 
        Zero; 

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
#   if XO_SSE
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 3.0f;
#   else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 3.0f;
#   endif
#endif

    ////////////////////////////////////////////////////////////////////////// Members
    // See: http://xo-math.rtfd.io/en/latest/classes/vector3.html#public_members
#if XO_SSE
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float f[4]; 
        __m128 m;
    };
#else
    union {
        struct {
            float x, y, z;
        };
        float f[3];
    };
#endif

private:
#if XO_SSE
    static const __m128 MASK;
#endif
};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector4.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Vector4 {
public:
    ////////////////////////////////////////////////////////////////////////// Constructors
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#constructors
    _XOINL Vector4(); 
    _XOINL Vector4(float f); 
    _XOINL Vector4(float x, float y, float z, float w); 
    _XOINL Vector4(const Vector4& vec); 
#if XO_SSE
    _XOINL Vector4(const __m128& vec); 
#endif
    _XOINL Vector4(const class Vector2& v); 
    _XOINL Vector4(const class Vector3& v); 

    ////////////////////////////////////////////////////////////////////////// Set / Get Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#set_get_methods
    _XOINL const Vector4& Set(float x, float y, float z, float w);
    _XOINL const Vector4& Set(float f);
    _XOINL const Vector4& Set(const Vector4& vec);
#if XO_SSE
    _XOINL const Vector4& Set(const __m128& vec);
#endif
    _XOINL void Get(float& x, float& y, float& z, float& w) const;
    _XOINL void Get(float* f) const;

    ////////////////////////////////////////////////////////////////////////// Special Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#special_operators
    _XO_OVERLOAD_NEW_DELETE();
#if XO_SSE
    _XOINL operator const __m128&() const;
#endif
    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;
    _XOINL Vector4 operator -() const;
    _XOINL Vector4 operator ~() const;

    ////////////////////////////////////////////////////////////////////////// Math Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#math_operators
    _XOINL const Vector4& operator += (const Vector4& v);
    _XOINL const Vector4& operator += (float v);
    _XOINL const Vector4& operator += (double v);
    _XOINL const Vector4& operator += (int v);
    _XOINL const Vector4& operator += (const class Vector2& v);
    _XOINL const Vector4& operator += (const class Vector3& v);
    _XOINL const Vector4& operator -= (const Vector4& v);
    _XOINL const Vector4& operator -= (float v);
    _XOINL const Vector4& operator -= (double v);
    _XOINL const Vector4& operator -= (int v);
    _XOINL const Vector4& operator -= (const class Vector2& v);
    _XOINL const Vector4& operator -= (const class Vector3& v);
    _XOINL const Vector4& operator *= (const Vector4& v);
    _XOINL const Vector4& operator *= (float v);
    _XOINL const Vector4& operator *= (double v);
    _XOINL const Vector4& operator *= (int v);
    _XOINL const Vector4& operator *= (const class Vector2& v);
    _XOINL const Vector4& operator *= (const class Vector3& v);
    // See: XO_NO_INVERSE_DIVISION
    _XOINL const Vector4& operator /= (const Vector4& v);
    _XOINL const Vector4& operator /= (float v);
    _XOINL const Vector4& operator /= (double v);
    _XOINL const Vector4& operator /= (int v);
    _XOINL const Vector4& operator /= (const class Vector2& v);
    _XOINL const Vector4& operator /= (const class Vector3& v);
    _XOINL Vector4 operator + (const Vector4& v) const;
    _XOINL Vector4 operator + (float v) const;
    _XOINL Vector4 operator + (double v) const;
    _XOINL Vector4 operator + (int v) const;
    _XOINL Vector4 operator + (const class Vector2& v) const;
    _XOINL Vector4 operator + (const class Vector3& v) const;
    _XOINL Vector4 operator - (const Vector4& v) const;
    _XOINL Vector4 operator - (float v) const;
    _XOINL Vector4 operator - (double v) const;
    _XOINL Vector4 operator - (int v) const;
    _XOINL Vector4 operator - (const class Vector2& v) const;
    _XOINL Vector4 operator - (const class Vector3& v) const;
    _XOINL Vector4 operator * (const Vector4& v) const;
    _XOINL Vector4 operator * (float v) const;
    _XOINL Vector4 operator * (double v) const;
    _XOINL Vector4 operator * (int v) const;
    _XOINL Vector4 operator * (const class Vector2& v) const;
    _XOINL Vector4 operator * (const class Vector3& v) const;
    // See: XO_NO_INVERSE_DIVISION
    _XOINL Vector4 operator / (const Vector4& v) const;
    _XOINL Vector4 operator / (float v) const;
    _XOINL Vector4 operator / (double v) const;
    _XOINL Vector4 operator / (int v) const;
    _XOINL Vector4 operator / (const class Vector2& v) const;
    _XOINL Vector4 operator / (const class Vector3& v) const;

    ////////////////////////////////////////////////////////////////////////// Comparison Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#comparison_operators
    _XOINL bool operator < (const Vector4& v) const;
    _XOINL bool operator < (float v) const;
    _XOINL bool operator < (double v) const;
    _XOINL bool operator < (int v) const;
    _XOINL bool operator < (const class Vector2& v) const;
    _XOINL bool operator < (const class Vector3& v) const;
    _XOINL bool operator <= (const Vector4& v) const;
    _XOINL bool operator <= (float v) const;
    _XOINL bool operator <= (double v) const;
    _XOINL bool operator <= (int v) const;
    _XOINL bool operator <= (const class Vector2& v) const;
    _XOINL bool operator <= (const class Vector3& v) const;
    _XOINL bool operator > (const Vector4& v) const;
    _XOINL bool operator > (float v) const;
    _XOINL bool operator > (double v) const;
    _XOINL bool operator > (int v) const;
    _XOINL bool operator > (const class Vector2& v) const;
    _XOINL bool operator > (const class Vector3& v) const;
    _XOINL bool operator >= (const Vector4& v) const;
    _XOINL bool operator >= (float v) const;
    _XOINL bool operator >= (double v) const;
    _XOINL bool operator >= (int v) const;
    _XOINL bool operator >= (const class Vector2& v) const;
    _XOINL bool operator >= (const class Vector3& v) const;

    _XOINL bool operator == (const Vector4& v) const;
    _XOINL bool operator == (float v) const;
    _XOINL bool operator == (double v) const;
    _XOINL bool operator == (int v) const;
    _XOINL bool operator == (const class Vector2& v) const;
    _XOINL bool operator == (const class Vector3& v) const;
    _XOINL bool operator != (const Vector4& v) const;
    _XOINL bool operator != (float v) const;
    _XOINL bool operator != (double v) const;
    _XOINL bool operator != (int v) const;
    _XOINL bool operator != (const class Vector2& v) const;
    _XOINL bool operator != (const class Vector3& v) const;


    _XOINL bool IsNormalized() const;
    _XOINL bool IsZero() const;
    _XOINL float Magnitude() const;
    _XOINL float MagnitudeSquared() const;
    _XOINL float Sum() const;

    _XOINL const Vector4& Normalize();

    _XOINL Vector4 Normalized() const;

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#static_methods
    _XOINL static void Lerp(const Vector4& a, const Vector4& b, float t, Vector4& outVec);
    _XOINL static void Max(const Vector4& a, const Vector4& b, Vector4& outVec);
    _XOINL static void Min(const Vector4& a, const Vector4& b, Vector4& outVec);
    _XOINL static float Distance(const Vector4&a, const Vector4&b);
    _XOINL static float DistanceSquared(const Vector4& a, const Vector4& b);
    _XOINL static float Dot(const Vector4& a, const Vector4& b);
    
    ////////////////////////////////////////////////////////////////////////// Variants
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#variants
    _XOINL static Vector4 Lerp(const Vector4& a, const Vector4& b, float t);
    _XOINL static Vector4 Max(const Vector4& a, const Vector4& b);
    _XOINL static Vector4 Min(const Vector4& a, const Vector4& b);

    _XOINL float Distance(const Vector4& v) const;
    _XOINL float DistanceSquared(const Vector4& v) const;
    _XOINL float Dot(const Vector4& v) const;
    _XOINL Vector4 Lerp(const Vector4& v, float t) const;

    ////////////////////////////////////////////////////////////////////////// Extras
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#extras
    friend std::ostream& operator <<(std::ostream& os, const Vector4& v) {
        os << "(x:" << v.x << ", y:" << v.y << ", z:" << v.z << ", w:" << v.w << ", mag:" << v.Magnitude() << ")";
        return os;
    }

    ////////////////////////////////////////////////////////////////////////// Static Attributes
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#public_static_attributes
    static const Vector4
        One, 
        Zero, 
        UnitX, 
        UnitY, 
        UnitZ, 
        UnitW; 

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
#   if XO_SSE
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

    ////////////////////////////////////////////////////////////////////////// Members
    // See: http://xo-math.rtfd.io/en/latest/classes/vector4.html#public_members
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float f[4]; 
#if XO_SSE
        __m128 m;
#endif
    };
};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Matrix4x4 {
public:
    //> See
    _XOINL Matrix4x4(); 
    _XOINL explicit Matrix4x4(float m); 
    _XOINL Matrix4x4(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33);
    _XOINL Matrix4x4(const Matrix4x4& m);
    _XOINL Matrix4x4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3);
    _XOINL Matrix4x4(const Vector3& r0, const Vector3& r1, const Vector3& r2);
    _XOINL Matrix4x4(const class Quaternion& q);


    _XOINL const Matrix4x4& SetRow(int i, const Vector4& r);
    _XOINL const Matrix4x4& SetColumn(int i, const Vector4& r);
    _XOINL const Vector4& GetRow(int i) const;
    _XOINL Vector4 GetColumn(int i) const;

    ////////////////////////////////////////////////////////////////////////// Special Operators
    // See: http://xo-math.rtfd.io/en/latest/classes/matrix4x4.html#special_operators
    _XO_OVERLOAD_NEW_DELETE();
    _XOINL const Vector4& operator [](int i) const;
    _XOINL Vector4& operator [](int i);
    _XOINL const float& operator ()(int r, int c) const;
    _XOINL float& operator ()(int r, int c);
    _XOINL Matrix4x4 operator ~() const;

    _XOINL const Matrix4x4& operator += (const Matrix4x4& m);
    _XOINL const Matrix4x4& operator -= (const Matrix4x4& m);
    _XOINL const Matrix4x4& operator *= (const Matrix4x4& m);

    _XOINL Matrix4x4 operator + (const Matrix4x4& m) const;
    _XOINL Matrix4x4 operator - (const Matrix4x4& m) const;
    _XOINL Matrix4x4 operator * (const Matrix4x4& m) const;

    _XOINL Vector4 operator * (const Vector4& v) const;

    _XOINL Vector3 operator * (const Vector3& v) const;


    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/matrix4x4.html#static_methods
    _XOINL const Matrix4x4& Inverse();
    _XOINL const Matrix4x4& Transpose();
    _XOINL const Matrix4x4& Transform(Vector3& v) const;
    _XOINL const Matrix4x4& Transform(Vector4& v) const;

    _XOINL Matrix4x4 Transposed() const;

    ////////////////////////////////////////////////////////////////////////// Static Methods
    // See: http://xo-math.rtfd.io/en/latest/classes/matrix4x4.html#static_methods
    _XOINL static void Scale(float xyz, Matrix4x4& outMatrix);
    _XOINL static void Scale(float x, float y, float z, Matrix4x4& outMatrix);
    _XOINL static void Scale(const Vector3& v, Matrix4x4& outMatrix);
    _XOINL static void Translation(float x, float y, float z, Matrix4x4& outMatrix);
    _XOINL static void Translation(const Vector3& v, Matrix4x4& outMatrix);
    _XOINL static void RotationXRadians(float radians, Matrix4x4& outMatrix);
    _XOINL static void RotationYRadians(float radians, Matrix4x4& outMatrix);
    _XOINL static void RotationZRadians(float radians, Matrix4x4& outMatrix);
    _XOINL static void RotationRadians(float x, float y, float z, Matrix4x4& outMatrix);
    _XOINL static void RotationRadians(const Vector3& v, Matrix4x4& outMatrix);
    _XOINL static void AxisAngleRadians(const Vector3& axis, float radians, Matrix4x4& outMatrix);
    _XOINL static void RotationXDegrees(float degrees, Matrix4x4& outMatrix);
    _XOINL static void RotationYDegrees(float degrees, Matrix4x4& outMatrix);
    _XOINL static void RotationZDegrees(float degrees, Matrix4x4& outMatrix);
    _XOINL static void RotationDegrees(float x, float y, float z, Matrix4x4& outMatrix);
    _XOINL static void RotationDegrees(const Vector3& v, Matrix4x4& outMatrix);
    _XOINL static void AxisAngleDegrees(const Vector3& axis, float degrees, Matrix4x4& outMatrix);
    _XOINL static void OrthographicProjection(float width, float height, float near, float far, Matrix4x4& outMatrix);
    _XOINL static void PerspectiveProjectionRadians(float fovx, float fovy, float near, float far, Matrix4x4& outMatrix);
    _XOINL static void PerspectiveProjectionDegrees(float fovx, float fovy, float near, float far, Matrix4x4& outMatrix);
    _XOINL static void LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Matrix4x4& outMatrix);
    _XOINL static void LookAtFromPosition(const Vector3& from, const Vector3& to, Matrix4x4& outMatrix);
    _XOINL static void LookAtFromDirection(const Vector3& direction, const Vector3& up, Matrix4x4& outMatrix);
    _XOINL static void LookAtFromDirection(const Vector3& direction, Matrix4x4& outMatrix);

    _XOINL static Matrix4x4 Scale(float xyz);
    _XOINL static Matrix4x4 Scale(float x, float y, float z);
    _XOINL static Matrix4x4 Scale(const Vector3& v);

    _XOINL static Matrix4x4 Translation(float x, float y, float z);
    _XOINL static Matrix4x4 Translation(const Vector3& v);
    
    _XOINL static Matrix4x4 RotationXRadians(float radians);
    _XOINL static Matrix4x4 RotationYRadians(float radians);
    _XOINL static Matrix4x4 RotationZRadians(float radians);
    _XOINL static Matrix4x4 RotationRadians(float x, float y, float z);
    _XOINL static Matrix4x4 RotationRadians(const Vector3& v);
    _XOINL static Matrix4x4 AxisAngleRadians(const Vector3& axis, float radians);

    _XOINL static Matrix4x4 RotationXDegrees(float degrees);
    _XOINL static Matrix4x4 RotationYDegrees(float degrees);
    _XOINL static Matrix4x4 RotationZDegrees(float degrees);
    _XOINL static Matrix4x4 RotationDegrees(float x, float y, float z);
    _XOINL static Matrix4x4 RotationDegrees(const Vector3& v);
    _XOINL static Matrix4x4 AxisAngleDegrees(const Vector3& axis, float degrees);

    _XOINL static Matrix4x4 OrthographicProjection(float width, float height, float near, float far);
    _XOINL static Matrix4x4 PerspectiveProjectionRadians(float fovx, float fovy, float near, float far);
    _XOINL static Matrix4x4 PerspectiveProjectionDegrees(float fovx, float fovy, float near, float far);

    _XOINL static Matrix4x4 LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up);
    _XOINL static Matrix4x4 LookAtFromPosition(const Vector3& from, const Vector3& to);
    _XOINL static Matrix4x4 LookAtFromDirection(const Vector3& direction, const Vector3& up);
    _XOINL static Matrix4x4 LookAtFromDirection(const Vector3& direction);

    ////////////////////////////////////////////////////////////////////////// Extras
    // See: http://xo-math.rtfd.io/en/latest/classes/matrix4x4.html#extras
    friend std::ostream& operator <<(std::ostream& os, const Matrix4x4& m) {
        os << "\nrow 0: " << m.r[0] << "\nrow 1: " << m.r[1] << "\nrow 2: " << m.r[2] << "\nrow 3: " << m.r[3] << "\n";
        return os;
    }

    Vector4 r[4];

    static const Matrix4x4
        Identity,
        One,
        Zero;
};


XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Quaternion.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

class _MM_ALIGN16 Quaternion {
public:
    _XOINL Quaternion();
    _XOINL Quaternion(const Matrix4x4& m);
    _XOINL Quaternion(float x, float y, float z, float w);

    _XO_OVERLOAD_NEW_DELETE();

    _XOINL float& operator [](int i);
    _XOINL const float& operator [](int i) const;

    _XOINL const Quaternion& operator *= (const Quaternion& q);
    _XOINL Quaternion operator * (const Quaternion& q) const;

    _XOINL bool operator == (const Quaternion& q) const;
    _XOINL bool operator != (const Quaternion& q) const;

    _XOINL const Quaternion& MakeConjugate();
    _XOINL const Quaternion& MakeInverse();
    _XOINL const Quaternion& Normalize();
    _XOINL Quaternion Conjugate() const;
    _XOINL Quaternion Inverse() const;
    _XOINL Quaternion Normalized() const;
    _XOINL void GetAxisAngleRadians(Vector3& axis, float& radians) const;

    _XOINL static void AxisAngleRadians(const Vector3& axis, float radians, Quaternion& outQuat);
    _XOINL static void Lerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat);
    _XOINL static void LookAtFromDirection(const Vector3& direction, const Vector3& up, Quaternion& outQuat);
    _XOINL static void LookAtFromDirection(const Vector3& direction, Quaternion& outQuat);
    _XOINL static void LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Quaternion& outQuat);
    _XOINL static void LookAtFromPosition(const Vector3& from, const Vector3& to, Quaternion& outQuat);
    _XOINL static void RotationRadians(const Vector3& v, Quaternion& outQuat);
    _XOINL static void RotationRadians(float x, float y, float z, Quaternion& outQuat);
    _XOINL static void Slerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat);

    _XOINL static Quaternion AxisAngleRadians(const Vector3& axis, float radians);
    _XOINL static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
    _XOINL static Quaternion LookAtFromDirection(const Vector3& direction);
    _XOINL static Quaternion LookAtFromDirection(const Vector3& direction, const Vector3& up);
    _XOINL static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to);
    _XOINL static Quaternion LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up);
    _XOINL static Quaternion RotationRadians(const Vector3& v);
    _XOINL static Quaternion RotationRadians(float x, float y, float z);
    _XOINL static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);

    static const Quaternion
        Identity,
        Zero;

#if defined(_XONOCONSTEXPR)
    static const float Epsilon;
#else
#   if XO_SSE
    _XOCONSTEXPR static const float Epsilon = sse::SSEFloatEpsilon * 4.0f;
#   else
    _XOCONSTEXPR static const float Epsilon = FloatEpsilon * 4.0f;
#   endif
#endif

    union {
        struct {
            float x, y, z, w;
        };
        float f[4];
#if XO_SSE
        __m128 m;
#endif
    };

private:
#if XO_SSE
    static const unsigned IDX_X = 0;
    static const unsigned IDX_Y = 1;
    static const unsigned IDX_Z = 2;
    static const unsigned IDX_W = 3;
#endif
};

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL


#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector2Operators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

float& Vector2::operator [](int i) { return f[i]; }
const float& Vector2::operator [](int i) const { return f[i]; }

Vector2 Vector2::operator - () const { return Vector2(-x, -y); }
Vector2 Vector2::operator ~ () const { return Vector2(y, x); }

const Vector2& Vector2::operator += (const Vector2& v) {
    x += v.x;
    y += v.y;
    return *this; 
}
const Vector2& Vector2::operator += (float v) {
    x += v;
    y += v;
    return *this; 
}
const Vector2& Vector2::operator += (double v)                  { return *this += (float)v; }
const Vector2& Vector2::operator += (int v)                     { return *this += (float)v; }
const Vector2& Vector2::operator += (const class Vector3& v)    { return *this += Vector2(v); }
const Vector2& Vector2::operator += (const class Vector4& v)    { return *this += Vector2(v); }

const Vector2& Vector2::operator -= (const Vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this; 
}
const Vector2& Vector2::operator -= (float v) {
    x -= v;
    y -= v;
    return *this; 
}
const Vector2& Vector2::operator -= (double v)                  { return *this -= (float)v; }
const Vector2& Vector2::operator -= (int v)                     { return *this -= (float)v; }
const Vector2& Vector2::operator -= (const class Vector3& v)    { return *this -= Vector2(v); }
const Vector2& Vector2::operator -= (const class Vector4& v)    { return *this -= Vector2(v); }

const Vector2& Vector2::operator *= (const Vector2& v) {
    x *= v.x;
    y *= v.y;
    return *this; 
}
const Vector2& Vector2::operator *= (float v) {
    x *= v;
    y *= v;
    return *this;
}
const Vector2& Vector2::operator *= (double v)                  { return *this *= (float)v; }
const Vector2& Vector2::operator *= (int v)                     { return *this *= (float)v; }
const Vector2& Vector2::operator *= (const class Vector3& v)    { return *this; }
const Vector2& Vector2::operator *= (const class Vector4& v)    { return *this; }

const Vector2& Vector2::operator /= (const Vector2& v) {
    x /= v.x;
    y /= v.y;
    return *this;
}
const Vector2& Vector2::operator /= (float v) {
#if defined(XO_NO_INVERSE_DIVISION)
    x /= v;
    y /= v;
    return *this;
#else
    return (*this) *= (1.0f/v);
#endif
    
}
const Vector2& Vector2::operator /= (double v)                  { return *this /= (float)v; }
const Vector2& Vector2::operator /= (int v)                     { return *this /= (float)v; }
const Vector2& Vector2::operator /= (const class Vector3& v)    { return *this /= Vector2(v); }
const Vector2& Vector2::operator /= (const class Vector4& v)    { return *this /= Vector2(v); }

Vector2 Vector2::operator + (const Vector2& v) const        { return Vector2(*this) += v; }
Vector2 Vector2::operator + (float v) const                 { return Vector2(*this) += v; }
Vector2 Vector2::operator + (double v) const                { return Vector2(*this) += v; }
Vector2 Vector2::operator + (int v) const                   { return Vector2(*this) += v; }
Vector2 Vector2::operator + (const class Vector3& v) const  { return Vector2(*this) += v; }
Vector2 Vector2::operator + (const class Vector4& v) const  { return Vector2(*this) += v; }

Vector2 Vector2::operator - (const Vector2& v) const        { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (float v) const                 { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (double v) const                { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (int v) const                   { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (const class Vector3& v) const  { return Vector2(*this) -= v; }
Vector2 Vector2::operator - (const class Vector4& v) const  { return Vector2(*this) -= v; }

Vector2 Vector2::operator * (const Vector2& v) const        { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (float v) const                 { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (double v) const                { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (int v) const                   { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (const class Vector3& v) const  { return Vector2(*this) *= v; }
Vector2 Vector2::operator * (const class Vector4& v) const  { return Vector2(*this) *= v; }

Vector2 Vector2::operator / (const Vector2& v) const        { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (float v) const                 { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (double v) const                { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (int v) const                   { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (const class Vector3& v) const  { return Vector2(*this) /= v; }
Vector2 Vector2::operator / (const class Vector4& v) const  { return Vector2(*this) /= v; }

bool Vector2::operator < (const Vector2& v) const           { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector2::operator < (float v) const                    { return MagnitudeSquared() < (v * v); }
bool Vector2::operator < (double v) const                   { return MagnitudeSquared() < (float)(v * v); }
bool Vector2::operator < (int v) const                      { return MagnitudeSquared() < (float)(v * v); }
bool Vector2::operator < (const class Vector3& v) const     { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector2::operator < (const class Vector4& v) const     { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector2::operator <= (const Vector2& v) const          { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector2::operator <= (float v) const                   { return MagnitudeSquared() <= (v * v); }
bool Vector2::operator <= (double v) const                  { return MagnitudeSquared() <= (float)(v * v); }
bool Vector2::operator <= (int v) const                     { return MagnitudeSquared() <= (float)(v * v); }
bool Vector2::operator <= (const class Vector3& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector2::operator <= (const class Vector4& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector2::operator > (const Vector2& v) const           { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector2::operator > (float v) const                    { return MagnitudeSquared() > (v * v); }
bool Vector2::operator > (double v) const                   { return MagnitudeSquared() > (float)(v * v); }
bool Vector2::operator > (int v) const                      { return MagnitudeSquared() > (float)(v * v); }
bool Vector2::operator > (const class Vector3& v) const     { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector2::operator > (const class Vector4& v) const     { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector2::operator >= (const Vector2& v) const          { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector2::operator >= (float v) const                   { return MagnitudeSquared() >= (v * v); }
bool Vector2::operator >= (double v) const                  { return MagnitudeSquared() >= (float)(v * v); }
bool Vector2::operator >= (int v) const                     { return MagnitudeSquared() >= (float)(v * v); }
bool Vector2::operator >= (const class Vector3& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector2::operator >= (const class Vector4& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector2::operator == (const Vector2& v) const {
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(x, v.x, Epsilon);
}
bool Vector2::operator == (float v) const                   { return CloseEnough(MagnitudeSquared(), v*v, Epsilon); }
bool Vector2::operator == (double v) const                  { return *this == (float)(v*v); }
bool Vector2::operator == (int v) const                     { return *this == (float)(v*v); }
bool Vector2::operator == (const class Vector3& v) const {
#   if XO_SSE2
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, y, x), v.m)), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}
bool Vector2::operator == (const class Vector4& v) const {
#   if XO_SSE
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, y, x), v.m)), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}

bool Vector2::operator != (const Vector2& v) const          { return !((*this) == v); }
bool Vector2::operator != (float v) const                   { return !((*this) == v); }
bool Vector2::operator != (double v) const                  { return !((*this) == v); }
bool Vector2::operator != (int v) const                     { return !((*this) == v); }
bool Vector2::operator != (const class Vector3& v) const    { return !((*this) == v); }
bool Vector2::operator != (const class Vector4& v) const    { return !((*this) == v); }

XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector2Methods.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

Vector2::Vector2() {
}

Vector2::Vector2(float v) : x(v), y(v) {
}

Vector2::Vector2(float x, float y) : x(x), y(y) {
}

Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) {
}

Vector2::Vector2(const class Vector3& v) : x(v.x), y(v.y) {
}

Vector2::Vector2(const class Vector4& v) : x(v.x), y(v.y) {
}

const Vector2& Vector2::Set(float x, float y) {
    this->x = x;
    this->y = y;
    return *this;
}

const Vector2& Vector2::Set(float v) {
    x = v;
    y = v;
    return *this;
}

const Vector2& Vector2::Set(const Vector2& v) {
    x = v.x;
    y = v.y;
    return *this;
}

void Vector2::Get(float& x, float& y) const {
    x = this->x;
    y = this->y;
}
void Vector2::Get(float* f) const {
    f[0] = this->x;
    f[1] = this->y;
}

float Vector2::Sum() const {
    return x+y;
}

float Vector2::Magnitude() const {
    return sqrtf(x*x + y*y);
}

float Vector2::MagnitudeSquared() const {
    return x*x + y*y;
}

const Vector2& Vector2::Normalize() {
    float magnitude = MagnitudeSquared();
    if (CloseEnough(magnitude, 1.0f, Epsilon))
        return *this; // already normalized
    if (CloseEnough(magnitude, 0.0f, Epsilon))
        return *this; // zero vec
    magnitude = sqrtf(magnitude);
    (*this) /= magnitude;
    return *this;
}

Vector2 Vector2::Normalized() const {
    return Vector2(*this).Normalize();
}

bool Vector2::IsZero() const {
    return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
}

bool Vector2::IsNormalized() const {
  // todo: check closeness
    return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
}


float Vector2::Dot(const Vector2& a, const Vector2& b) {
    return (a.x * b.x) + (a.y * b.y);
}

float Vector2::Cross(const Vector2& a, const Vector2& b) {
    return (a.x * b.y) - (a.y * b.x);
}


float Vector2::AngleRadians(const Vector2& a, const Vector2& b) {
    return -ATan2(Cross(a, b), Dot(a, b));
}

float Vector2::AngleDegrees(const Vector2& a, const Vector2& b) {
    return AngleRadians(a, b) * Rad2Deg;
}

float Vector2::Distance(const Vector2& a, const Vector2& b) {
    return (b-a).Magnitude();
}

float Vector2::DistanceSquared(const Vector2& a, const Vector2& b) {
    return (b-a).MagnitudeSquared();
}

void Vector2::Max(const Vector2& a, const Vector2& b, Vector2& outVec) {
    outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y));
}

void Vector2::Min(const Vector2& a, const Vector2& b, Vector2& outVec) {
    outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y));
}

void Vector2::OrthogonalCCW(const Vector2& v, Vector2& outVec) {
    outVec.Set(-v.y, v.x);
}

void Vector2::OrthogonalCW(const Vector2& v, Vector2& outVec) {
    outVec.Set(v.y, -v.x);
}

void Vector2::Lerp(const Vector2& a, const Vector2& b, float t, Vector2& outVec) {
    if(CloseEnough(t, 0.0f, Epsilon)) {
        outVec.Set(a);
    }
    else if(CloseEnough(t, 1.0f, Epsilon)) {
        outVec.Set(b);
    }
    else {
        outVec.Set(a + ((b - a) * t));
    }
}

void Vector2::Midpoint(const Vector2& a, const Vector2& b, Vector2& outVec) {
    Vector2::Lerp(a, b, 0.5f, outVec);
}

Vector2 Vector2::Max(const Vector2& a, const Vector2& b) {
    Vector2 tempVec;
    Max(a, b, tempVec);
    return tempVec;
}

Vector2 Vector2::Min(const Vector2& a, const Vector2& b) {
    Vector2 tempVec;
    Min(a, b, tempVec);
    return tempVec;
}

Vector2 Vector2::OrthogonalCCW(const Vector2& v) {
    Vector2 tempVec;
    OrthogonalCCW(v, tempVec);
    return tempVec;
}

Vector2 Vector2::OrthogonalCW(const Vector2& v) {
    Vector2 tempVec;
    OrthogonalCW(v, tempVec);
    return tempVec;
}

Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t) {
    Vector2 tempVec;
    Lerp(a, b, t, tempVec);
    return tempVec;
}

Vector2 Vector2::Midpoint(const Vector2& a, const Vector2& b) {
    Vector2 tempVec;
    Midpoint(a, b, tempVec);
    return tempVec;
}

float Vector2::AngleDegrees(const Vector2& v) const         { return AngleDegrees(*this, v); }
float Vector2::AngleRadians(const Vector2& v) const         { return AngleRadians(*this, v); }
float Vector2::Cross(const Vector2& v) const                { return Cross(*this, v); }
float Vector2::Distance(const Vector2& v) const             { return Distance(*this, v); }
float Vector2::DistanceSquared(const Vector2& v) const      { return DistanceSquared(*this, v); }
float Vector2::Dot(const Vector2& v) const                  { return Dot(*this, v); }
Vector2 Vector2::Lerp(const Vector2& v, float t) const      { return Lerp(*this, v, t); }
Vector2 Vector2::Midpoint(const Vector2& v) const           { return Midpoint(*this, v); }
Vector2 Vector2::OrthogonalCCW() const                      { return OrthogonalCCW(*this); }
Vector2 Vector2::OrthogonalCW() const                       { return OrthogonalCW(*this); }

XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3Operators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

#if XO_SSE
Vector3::operator __m128() const {
    return m;
}
#endif

float& Vector3::operator [](int i) {
    return f[i];
}

const float& Vector3::operator [](int i) const {
    return f[i];
}

Vector3 Vector3::operator -() const {
#if XO_SSE
    return Vector3(_mm_mul_ps(m, sse::NegativeOne));
#else
    return Vector3(-x, -y, -z);
#endif
}

Vector3 Vector3::operator ~() const {
    return ZYX();
}

const Vector3& Vector3::operator += (const Vector3& v) {
#if XO_SSE
    m = _mm_add_ps(m, v);
#else
    x += v.x;
    y += v.y;
    z += v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator += (float v) {
#if XO_SSE
    m = _mm_add_ps(m, _mm_set_ps1(v));
#else
    x += v;
    y += v;
    z += v;
#endif
    return *this;
}

const Vector3& Vector3::operator += (double v)                  { return (*this) += float(v); }
const Vector3& Vector3::operator += (int v)                     { return (*this) += float(v); }
const Vector3& Vector3::operator += (const class Vector2& v)    { return (*this) += Vector3(v); }
const Vector3& Vector3::operator += (const class Vector4& v)    { return (*this) += Vector3(v); }

const Vector3& Vector3::operator -= (const Vector3& v) {
#if XO_SSE
    m = _mm_sub_ps(m, v);
#else
    x -= v.x;
    y -= v.y;
    z -= v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator -= (float v) {
#if XO_SSE
    m = _mm_sub_ps(m, _mm_set_ps1(v));
#else
    x -= v;
    y -= v;
    z -= v;
#endif
    return *this;
}

const Vector3& Vector3::operator -= (double v)                  { return (*this) -= float(v); }
const Vector3& Vector3::operator -= (int v)                     { return (*this) -= float(v); }
const Vector3& Vector3::operator -= (const class Vector2& v)    { return (*this) -= Vector3(v); }
const Vector3& Vector3::operator -= (const class Vector4& v)    { return (*this) -= Vector3(v); }

const Vector3& Vector3::operator *= (const Vector3& v) {
#if XO_SSE
    m = _mm_mul_ps(m, v);
#else
    x *= v.x;
    y *= v.y;
    z *= v.z;
#endif
    return *this;
}

const Vector3& Vector3::operator *= (float v) {
#if XO_SSE
    m = _mm_mul_ps(m, _mm_set_ps1(v));
#else
    x *= v;
    y *= v;
    z *= v;
#endif
    return *this;
}

const Vector3& Vector3::operator *= (double v)                  { return (*this) *= float(v); }
const Vector3& Vector3::operator *= (int v)                     { return (*this) *= float(v); }
const Vector3& Vector3::operator *= (const class Vector2& v)    { return (*this) *= Vector3(v); }
const Vector3& Vector3::operator *= (const class Vector4& v)    { return (*this) *= Vector3(v); }

#if defined(XO_NO_INVERSE_DIVISION)
const Vector3& Vector3::operator /= (const Vector3& v) {
#if XO_SSE
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
    m = _mm_div_ps(m, v);
#else
    x /= v.x;
    y /= v.y;
    z /= v.z;
#endif

    return *this;
}

const Vector3& Vector3::operator /= (float v) {
#   if XO_SSE
    m = _mm_div_ps(m, _mm_set_ps1(v));
#   else
    x /= v;
    y /= v;
    z /= v;
#   endif
    return *this;
}
#else

const Vector3& Vector3::operator /= (const Vector3& v) {
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
    m = _mm_mul_ps(m, _mm_rcp_ps(v));
#   else
    x /= v.x;
    y /= v.y;
    z /= v.z;
#   endif

    return *this;
}

const Vector3& Vector3::operator /= (float v) { 
#   if XO_SSE
    m = _mm_mul_ps(m, _mm_set_ps1(1.0f/v));
#   else
    v = 1.0f / v;
    x *= v;
    y *= v;
    z *= v;
#   endif
    return *this;
}
#endif
const Vector3& Vector3::operator /= (double v)                  { return (*this) /= float(v); }
const Vector3& Vector3::operator /= (int v)                     { return (*this) /= float(v); }
const Vector3& Vector3::operator /= (const class Vector2& v)    { return (*this) /= Vector3(v); }
const Vector3& Vector3::operator /= (const class Vector4& v)    { return (*this) /= Vector3(v); }

Vector3 Vector3::operator + (const Vector3& v) const        { return Vector3(*this) += v; }
Vector3 Vector3::operator + (float v) const                 { return Vector3(*this) += v; }
Vector3 Vector3::operator + (double v) const                { return Vector3(*this) += v; }
Vector3 Vector3::operator + (int v) const                   { return Vector3(*this) += v; }
Vector3 Vector3::operator + (const class Vector2& v) const  { return Vector3(*this) += v; }
Vector3 Vector3::operator + (const class Vector4& v) const  { return Vector3(*this) += v; }

Vector3 Vector3::operator - (const Vector3& v) const        { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (float v) const                 { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (double v) const                { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (int v) const                   { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (const class Vector2& v) const  { return Vector3(*this) -= v; }
Vector3 Vector3::operator - (const class Vector4& v) const  { return Vector3(*this) -= v; }

Vector3 Vector3::operator * (const Vector3& v) const        { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (float v) const                 { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (double v) const                { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (int v) const                   { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (const class Vector2& v) const  { return Vector3(*this) *= v; }
Vector3 Vector3::operator * (const class Vector4& v) const  { return Vector3(*this) *= v; }

Vector3 Vector3::operator / (const Vector3& v) const        { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (float v) const                 { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (double v) const                { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (int v) const                   { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (const class Vector2& v) const  { return Vector3(*this) /= v; }
Vector3 Vector3::operator / (const class Vector4& v) const  { return Vector3(*this) /= v; }


bool Vector3::operator < (const Vector3& v) const       { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector3::operator < (float v) const                { return MagnitudeSquared() < (v * v); }
bool Vector3::operator < (double v) const               { return MagnitudeSquared() < (float)(v * v); }
bool Vector3::operator < (int v) const                  { return MagnitudeSquared() < (float)(v * v); }
bool Vector3::operator < (const class Vector2& v) const { return MagnitudeSquared() < v.MagnitudeSquared(); }
bool Vector3::operator < (const class Vector4& v) const { return MagnitudeSquared() < v.MagnitudeSquared(); }

bool Vector3::operator <= (const Vector3& v) const          { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector3::operator <= (float v) const                   { return MagnitudeSquared() <= (v * v); }
bool Vector3::operator <= (double v) const                  { return MagnitudeSquared() <= (float)(v * v); }
bool Vector3::operator <= (int v) const                     { return MagnitudeSquared() <= (float)(v * v); }
bool Vector3::operator <= (const class Vector2& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }
bool Vector3::operator <= (const class Vector4& v) const    { return MagnitudeSquared() <= v.MagnitudeSquared(); }

bool Vector3::operator > (const Vector3& v) const       { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector3::operator > (float v) const                { return MagnitudeSquared() > (v * v); }
bool Vector3::operator > (double v) const               { return MagnitudeSquared() > (float)(v * v); }
bool Vector3::operator > (int v) const                  { return MagnitudeSquared() > (float)(v * v); }
bool Vector3::operator > (const class Vector2& v) const { return MagnitudeSquared() > v.MagnitudeSquared(); }
bool Vector3::operator > (const class Vector4& v) const { return MagnitudeSquared() > v.MagnitudeSquared(); }

bool Vector3::operator >= (const Vector3& v) const          { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector3::operator >= (float v) const                   { return MagnitudeSquared() >= (v * v); }
bool Vector3::operator >= (double v) const                  { return MagnitudeSquared() >= (float)(v * v); }
bool Vector3::operator >= (int v) const                     { return MagnitudeSquared() >= (float)(v * v); }
bool Vector3::operator >= (const class Vector2& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }
bool Vector3::operator >= (const class Vector4& v) const    { return MagnitudeSquared() >= v.MagnitudeSquared(); }

bool Vector3::operator == (const Vector3& v) const {
#   if XO_SSE2
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v, m)), sse::Epsilon)) & 7) == 7;
#   elif XO_SSE
    // TODO: find a faster way with SSE to do a 'close enough' check.
    // I'm not sure if there's a way to do the sign bit masking like we have in sse::Abs to acomplish
    // what we're doing in SSE2
    return CloseEnough(x, v.x, sse::SSEFloatEpsilon) && CloseEnough(y, v.y, sse::SSEFloatEpsilon) && CloseEnough(z, v.z, sse::SSEFloatEpsilon);
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon) && CloseEnough(z, v.z, Epsilon);
#   endif
}
bool Vector3::operator == (float v) const                   { return CloseEnough(MagnitudeSquared(), v*v, Epsilon);}
bool Vector3::operator == (double v) const                  { return CloseEnough(MagnitudeSquared(), (float)(v*v), Epsilon);}
bool Vector3::operator == (int v) const                     { return CloseEnough(MagnitudeSquared(), (float)(v*v), Epsilon);}
bool Vector3::operator == (const class Vector2& v) const {
#   if XO_SSE
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(_mm_set_ps(0.0f, 0.0f, v.y, v.x), m)), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}
bool Vector3::operator == (const class Vector4& v) const {
#   if XO_SSE
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.m, m)), sse::Epsilon)) & 7) == 7;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon) && CloseEnough(z, v.z, Epsilon);
#   endif
}

bool Vector3::operator != (const Vector3& v) const          { return !((*this) == v); }
bool Vector3::operator != (float v) const                   { return !((*this) == v); }
bool Vector3::operator != (double v) const                  { return !((*this) == v); }
bool Vector3::operator != (int v) const                     { return !((*this) == v); }
bool Vector3::operator != (const class Vector2& v) const    { return !((*this) == v); }
bool Vector3::operator != (const class Vector4& v) const    { return !((*this) == v); }

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector3Methods.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

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

Vector3::Vector3() {
}

Vector3::Vector3(float f) :
#if XO_SSE
    m(_mm_set1_ps(f))
#else
    x(f), y(f), z(f)
#endif
{
}

Vector3::Vector3(float x, float y, float z) :
#if XO_SSE
    m(_mm_set_ps(0.0f, z, y, x))
#else
    x(x), y(y), z(z)
#endif
{
}

Vector3::Vector3(const Vector3& vec) :
#if XO_SSE
    m(vec) 
#else
    x(vec.x), y(vec.y), z(vec.z) 
#endif
{
}

#if XO_SSE
Vector3::Vector3(const __m128& vec) : 
    m(vec) 
{
}
#endif

Vector3::Vector3(const class Vector2& v) :
#if XO_SSE
    m(_mm_set_ps(0.0f, 0.0f, v.y, v.x))
#else
    x(v.x), y(v.y), z(0.0f)
#endif
{
}

Vector3::Vector3(const class Vector4& v) :
#if XO_SSE
    m(v.m)
#else
    x(v.x), y(v.y), z(v.z)
#endif
{
}

const Vector3& Vector3::Set(float x, float y, float z) {
#if XO_SSE
    m = _mm_set_ps(0.0f, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
#endif
    return *this;
}

const Vector3& Vector3::Set(float f) {
#if XO_SSE
    m = _mm_set1_ps(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
#endif
    return *this;
}

const Vector3& Vector3::Set(const Vector3& vec) {
#if XO_SSE
    m = vec.m;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
#endif
    return *this;
}

#if XO_SSE
const Vector3& Vector3::Set(const __m128& vec) {
    m = vec;
    return *this;
}
#endif

void Vector3::Get(float& x, float& y, float &z) const {
    x = this->x;
    y = this->y;
    z = this->z;
}

void Vector3::Get(float* f) const {
#if XO_SSE
    _mm_store_ps(f, m);
#else
    f[0] = this->x;
    f[1] = this->y;
    f[2] = this->z;
#endif
}

Vector3 Vector3::ZYX() const {
#if XO_SSE
    return Vector3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Y, IDX_Z)));
#else
    return Vector3(z, y, x);
#endif
}

_XOINL float Vector3::Sum() const {
#if XO_SSE3
    __m128 x = _mm_and_ps(m, MASK);
    x = _mm_hadd_ps(x, x);
    x = _mm_hadd_ps(x, x);
    return _mm_cvtss_f32(x);
#else
    return x+y+z;
#endif
}

float Vector3::MagnitudeSquared() const {
    Vector3 x = *this;
    x *= x;
    return x.Sum();
}

float Vector3::Magnitude() const {
    return Sqrt(MagnitudeSquared());
}

const Vector3& Vector3::Normalize() {
    float magnitude = MagnitudeSquared();
    if (CloseEnough(magnitude, 1.0f, Epsilon)) {
        return *this; // already normalized
    }
    else if (CloseEnough(magnitude, 0.0f, Epsilon)) {
        return *this; // zero vec
    }
    else {
        magnitude = Sqrt(magnitude);
        return (*this) /= magnitude;
    }
}


Vector3 Vector3::Normalized() const {
    return Vector3(*this).Normalize();
}

bool Vector3::IsZero() const {
    return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
}


bool Vector3::IsNormalized() const {
    return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
}
 
float Vector3::Dot(const Vector3& a, const Vector3& b) {
#if XO_SSE4_1
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x7f));
#elif XO_SSE3
    auto d = _mm_and_ps(_mm_mul_ps(a.m, b.m), MASK);
    d = _mm_hadd_ps(d, d);
    d = _mm_hadd_ps(d, d);
    return _mm_cvtss_f32(d);
#elif XO_SSE
    auto d = _mm_mul_ps(a.m, b.m);
    _MM_ALIGN16 float t[4];
    _mm_store_ps(t, d);
    return t[IDX_X] + t[IDX_Y] + t[IDX_Z];
#else
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
#endif
}
 
void Vector3::Cross(const Vector3& a, const Vector3& b, Vector3& outVec) {
#if XO_SSE
    // Todo: There's a trick to do this with three shuffles. Look into that.
    __m128 l = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)));
    __m128 r = _mm_mul_ps(_mm_shuffle_ps(a.m, a.m, _MM_SHUFFLE(IDX_W, IDX_Y, IDX_X, IDX_Z)), _mm_shuffle_ps(b.m, b.m, _MM_SHUFFLE(IDX_W, IDX_X, IDX_Z, IDX_Y)));
    outVec.m = _mm_sub_ps(l, r);
#else
    outVec.x = (a.y*b.z)-(a.z*b.y);
    outVec.y = (a.z*b.x)-(a.x*b.z);
    outVec.z = (a.x*b.y)-(a.y*b.x);
#endif
}

void Vector3::Max(const Vector3& a, const Vector3& b, Vector3& outVec) {
    outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y), _XO_MAX(a.z, b.z));
}
 
void Vector3::Min(const Vector3& a, const Vector3& b, Vector3& outVec) {
    outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y), _XO_MIN(a.z, b.z));
}

void Vector3::Lerp(const Vector3& a, const Vector3& b, float t, Vector3& outVec) {
    if(CloseEnough(t, 0.0f)) {
        outVec.Set(a);
    }
    else if(CloseEnough(t, 1.0f)) {
        outVec.Set(b);
    } 
    else {
        outVec.Set(a + ((b - a) * t));
    }
}

void Vector3::RotateRadians(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec) {
    // Rodrigues' rotation formula
    // https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
    Vector3 axv;
    float sinAng = Sin(angle);
    float cosAng = Cos(angle);
    Vector3::Cross(axis, v, axv);
    float adv = Vector3::Dot(axis, v);
    outVec.Set(v * cosAng + axv * sinAng + axis * adv * (1.0f - cosAng));
}

void Vector3::RotateDegrees(const Vector3& v, const Vector3& axis, float angle, Vector3& outVec) {
    RotateRadians(v, axis, angle * Deg2Rad, outVec);
}

void Vector3::RandomOnConeRadians(const Vector3& forward, float angle, Vector3& outVec) {
    Vector3 forwardNorm = forward.Normalized();
    Vector3 crossed, tilted;
    Vector3 other = forwardNorm == Up ? Left : Up; // anything unit length but the forwardNorm vector
    Vector3::Cross(forwardNorm, other, crossed);
    Vector3::RotateRadians(forwardNorm, crossed, angle, tilted);
    Vector3::RotateRadians(tilted, forwardNorm, RandomRange(-PI, PI), outVec);
    outVec *= forward.Magnitude();
}

void Vector3::RandomInConeRadians(const Vector3& forward, float angle, Vector3& outVec) {
    Vector3 forwardNorm = forward.Normalized();
    Vector3 crossed, tilted;
    Vector3 other = forwardNorm == Up ? Left : Up; // anything unit length but the forwardNorm vector
    Vector3::Cross(forwardNorm, other, crossed);
    Vector3::RotateRadians(forwardNorm, crossed, RandomRange(0.0f, angle), tilted);
    Vector3::RotateRadians(tilted, forwardNorm, RandomRange(-PI, PI), outVec);
    outVec *= forward.Magnitude();
}

void Vector3::RandomOnConeDegrees(const Vector3& forward, float angle, Vector3& outVec) {
    RandomOnConeRadians(forward, angle * Deg2Rad, outVec);
}

void Vector3::RandomInConeDegrees(const Vector3& forward, float angle, Vector3& outVec) {
    RandomInConeRadians(forward, angle * Deg2Rad, outVec);
}

void Vector3::RandomOnSphere(Vector3& outVec) {
    // TODO: find a better method to do this.
    // In the off chance we randomly pick 0,0,0 this method will break.
    outVec.Set(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
    outVec.Normalize();
}

void Vector3::RandomInSphere(Vector3& outVec) {
    RandomOnSphere(outVec);
    outVec *= RandomRange(0.0f, 1.0f);
}

void Vector3::RandomAtDistance(float d, Vector3& outVec) {
    RandomOnSphere(outVec);
    outVec *= d;
}

void Vector3::RandomInDistance(float d, Vector3& outVec) {
    RandomOnSphere(outVec);
    outVec *= RandomRange(0.0f, d);
}

void Vector3::RandomInRange(float low, float high, Vector3& outVec) {
    RandomOnSphere(outVec);
    outVec *= RandomRange(low, high);
}

float Vector3::AngleRadians(const Vector3& a, const Vector3& b) {
    Vector3 cross;
    Vector3::Cross(a, b, cross);
    cross *= cross;
    return ATan2(Sqrt(cross.Sum()), Vector3::Dot(a, b));
}

float Vector3::AngleDegrees(const Vector3& a, const Vector3& b) {
    return AngleRadians(a, b) * Rad2Deg;
}

float Vector3::DistanceSquared(const Vector3& a, const Vector3& b) {
    return (b - a).MagnitudeSquared();
}
 
float Vector3::Distance(const Vector3&a, const Vector3&b) {
    return (b - a).Magnitude();
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
    Vector3 tempV;
    Cross(a, b, tempV);
    return tempV;
}
    
Vector3 Vector3::Max(const Vector3& a, const Vector3& b) {
    Vector3 tempV;
    Max(a, b, tempV);
    return tempV;
}
    
Vector3 Vector3::Min(const Vector3& a, const Vector3& b) {
    Vector3 tempV;
    Min(a, b, tempV);
    return tempV;
}
    
Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t) {
    Vector3 tempV;
    Lerp(a, b, t, tempV);
    return tempV;
}
    
Vector3 Vector3::RotateRadians(const Vector3& v, const Vector3& axis, float angle) {
    Vector3 tempV;
    RotateRadians(v, axis, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RotateDegrees(const Vector3& v, const Vector3& axis, float angle) {
    Vector3 tempV;
    RotateDegrees(v, axis, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomOnConeRadians(const Vector3& forward, float angle) {
    Vector3 tempV;
    RandomOnConeRadians(forward, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInConeRadians(const Vector3& forward, float angle) {
    Vector3 tempV;
    RandomInConeRadians(forward, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomOnConeDegrees(const Vector3& forward, float angle) {
    Vector3 tempV;
    RandomOnConeDegrees(forward, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInConeDegrees(const Vector3& forward, float angle) {
    Vector3 tempV;
    RandomInConeDegrees(forward, angle, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomOnSphere() {
    Vector3 tempV;
    RandomOnSphere(tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInSphere() {
    Vector3 tempV;
    RandomInSphere(tempV);
    return tempV;
}
    
Vector3 Vector3::RandomAtDistance(float d) {
    Vector3 tempV;
    RandomAtDistance(d, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInDistance(float d) {
    Vector3 tempV;
    RandomInDistance(d, tempV);
    return tempV;
}
    
Vector3 Vector3::RandomInRange(float low, float high) {
    Vector3 tempV;
    RandomInRange(low, high, tempV);
    return tempV;
}

float Vector3::Dot(const Vector3& v) const                                { return Dot(*this, v); }

Vector3 Vector3::Cross(const Vector3& v) const                            { return Cross(*this, v); }

float Vector3::AngleRadians(const Vector3& v) const                       { return AngleRadians(*this, v); }

float Vector3::AngleDegrees(const Vector3& v) const                       { return AngleDegrees(*this, v); }

float Vector3::DistanceSquared(const Vector3& v) const                    { return DistanceSquared(*this, v); }

float Vector3::Distance(const Vector3& v) const                           { return Distance(*this, v); }

Vector3 Vector3::Lerp(const Vector3& v, float t) const                    { return Lerp(*this, v, t); }

Vector3 Vector3::RotateRadians(const Vector3& axis, float angle) const    { return RotateRadians(*this, axis, angle); }

Vector3 Vector3::RotateDegrees(const Vector3& axis, float angle) const    { return RotateDegrees(*this, axis, angle); }

Vector3 Vector3::RandomOnConeRadians(float angle) const                   { return RandomOnConeRadians(*this, angle); }

Vector3 Vector3::RandomInConeRadians(float angle) const                   { return RandomInConeRadians(*this, angle); }

Vector3 Vector3::RandomOnConeDegrees(float angle) const                   { return RandomOnConeDegrees(*this, angle); }

Vector3 Vector3::RandomInConeDegrees(float angle) const                   { return RandomInConeDegrees(*this, angle); }

#undef IDX_X
#undef IDX_Y
#undef IDX_Z
#undef IDX_W

XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector4Operators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

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
const Vector4& Vector4::operator += (const class Vector2& v)  { return (*this) += Vector4(v); }
const Vector4& Vector4::operator += (const class Vector3& v)  { return (*this) += Vector4(v); }

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
const Vector4& Vector4::operator -= (const class Vector2& v)  { return (*this) -= Vector4(v); }
const Vector4& Vector4::operator -= (const class Vector3& v)  { return (*this) -= Vector4(v); }

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

const Vector4& Vector4::operator *= (double v)          { return (*this) *= (float)v; }
const Vector4& Vector4::operator *= (int v)             { return (*this) *= (float)v; }
const Vector4& Vector4::operator *= (const class Vector2& v)  { return (*this) *= Vector4(v); }
const Vector4& Vector4::operator *= (const class Vector3& v)  { return (*this) *= Vector4(v); }

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
const Vector4& Vector4::operator /= (const class Vector2& v)  { return (*this) /= Vector4(v); }
const Vector4& Vector4::operator /= (const class Vector3& v)  { return (*this) /= Vector4(v); }

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
#   if XO_SSE2
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.m, m)), sse::Epsilon)) & 15) == 15;
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
#   if XO_SSE
    // Todo: check that this is actually faster.
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(m, _mm_set_ps(0.0f, 0.0f, v.y, v.x))), sse::Epsilon)) & 3) == 3;
#   else
    return CloseEnough(x, v.x, Epsilon) && CloseEnough(y, v.y, Epsilon);
#   endif
}

bool Vector4::operator == (const class Vector3& v) const {
#   if XO_SSE
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(v.m, m)), sse::Epsilon)) & 7) == 7;
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

#undef IDX_X
#undef IDX_Y
#undef IDX_Z
#undef IDX_W

XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Vector4Methods.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

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

Vector4::Vector4() {
}

Vector4::Vector4(float f) :
#if XO_SSE
    m(_mm_set1_ps(f))
{
}
#else
    x(f), y(f), z(f), w(f)
{
}
#endif

Vector4::Vector4(float x, float y, float z, float w) :
#if XO_SSE
    m(_mm_set_ps(w, z, y, x))
{
}
#else
    x(x), y(y), z(z), w(w)
{
}
#endif
Vector4::Vector4(const Vector4& vec) :
#if XO_SSE
    m(vec.m)
{
}
#else
    x(vec.x), y(vec.y), z(vec.z), w(vec.w)
{
}
#endif


#if XO_SSE
Vector4::Vector4(const __m128& vec) : 
    m(vec)
{
}
#endif

Vector4::Vector4(const class Vector2& vec) :
#if XO_SSE
    m(_mm_set_ps(0.0f, 0.0f, vec.y, vec.x)) 
{
}
#else
    x(vec.x), y(vec.y), z(0.0f), w(0.0f)
{
}
#endif

Vector4::Vector4(const class Vector3& vec) :
#if XO_SSE
    m(vec.m)
{
}
#else
    x(vec.x), y(vec.y), z(vec.z), w(0.0f)
{
}
#endif

const Vector4& Vector4::Set(float x, float y, float z, float w) {
#if XO_SSE
    m = _mm_set_ps(w, z, y, x);
#else
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
#endif
    return *this;
}

const Vector4& Vector4::Set(float f) {
#if XO_SSE
    m = _mm_set1_ps(f);
#else
    this->x = f;
    this->y = f;
    this->z = f;
    this->w = w;
#endif
    return *this;
}

const Vector4& Vector4::Set(const Vector4& vec) {
#if XO_SSE
    m = vec.m;
#else
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = vec.w;
#endif
    return *this;
}

#if XO_SSE
const Vector4& Vector4::Set(const __m128& vec) {
    m = vec;
    return *this;
}
#endif

void Vector4::Get(float& x, float& y, float& z, float& w) const {
    x = this->x;
    y = this->y;
    z = this->z;
    w = this->w;
}

void Vector4::Get(float* f) const {
#if XO_SSE
    _mm_store_ps(f, m);
#else
    f[0] = this->x;
    f[1] = this->y;
    f[2] = this->z;
    f[3] = this->w;
#endif
}
 
float Vector4::Sum() const {
#if XO_SSE
    auto s = _mm_hadd_ps(m, m);
    s = _mm_hadd_ps(s, s);
    return _mm_cvtss_f32(s);
#else
    return x+y+z+w;
#endif
}
 
float Vector4::MagnitudeSquared() const {
#if XO_SSE
    auto square = _mm_mul_ps(m, m);
    square = _mm_hadd_ps(square, square);
    square = _mm_hadd_ps(square, square);
    return _mm_cvtss_f32(square);
#else
    return (x*x) + (y*y) + (z*z) + (w*w);
#endif
}
 
float Vector4::Magnitude() const {
    return Sqrt(MagnitudeSquared());
}

const Vector4& Vector4::Normalize() {
    float magnitude = MagnitudeSquared();
    if (CloseEnough(magnitude, 1.0f, Epsilon)) {
        return *this; // already normalized
    }
    else if (CloseEnough(magnitude, 0.0f, Epsilon)) {
        return *this; // zero vec
    }
    else {
        magnitude = Sqrt(magnitude);
        return (*this) /= magnitude;
    }
}
 
Vector4 Vector4::Normalized() const {
    return Vector4(*this).Normalize();
}

bool Vector4::IsZero() const {
    return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
}

bool Vector4::IsNormalized() const {
    return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
}
 
void Vector4::Max(const Vector4& a, const Vector4& b, Vector4& outVec) {
    outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y), _XO_MAX(a.z, b.z), _XO_MAX(a.w, b.w));
}

void Vector4::Min(const Vector4& a, const Vector4& b, Vector4& outVec) {
    outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y), _XO_MIN(a.z, b.z), _XO_MIN(a.w, b.w));
}

void Vector4::Lerp(const Vector4& a, const Vector4& b, float t, Vector4& outVec) {
    if(CloseEnough(t, 0.0f, Epsilon)) {
        outVec = a;
    }
    else if(CloseEnough(t, 1.0f, Epsilon)) {
        outVec = b;
    }
    else {
        outVec = a + ((b - a) * t);    
    }
}

float Vector4::Dot(const Vector4& a, const Vector4& b) {
#if defined(XO_SSE4)
    return _mm_cvtss_f32(_mm_dp_ps(a.m, b.m, 0xff));
#elif defined(XO_SSE3)
    __m128 d = _mm_mul_ps(a.m, b.m);
    d = _mm_hadd_ps(d, d);
    d = _mm_hadd_ps(d, d);
    return _mm_cvtss_f32(d);
#elif XO_SSE
    __m128 d = _mm_mul_ps(a.m, b.m);
    _MM_ALIGN16 float t[4];
    _mm_store_ps(t, d);
    return t[IDX_X] + t[IDX_Y] + t[IDX_Z] + t[IDX_W];
#else
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z) + (a.w*b.w);
#endif
}


float Vector4::DistanceSquared(const Vector4& a, const Vector4& b) {
    return (b - a).MagnitudeSquared();
}

float Vector4::Distance(const Vector4&a, const Vector4&b) {
    return (b - a).Magnitude();
}

Vector4 Vector4::Max(const Vector4& a, const Vector4& b) {
    Vector4 v;
    Max(a, b, v);
    return v;
}

Vector4 Vector4::Min(const Vector4& a, const Vector4& b) {
    Vector4 v;
    Min(a, b, v);
    return v;
}

Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, float t) {
    Vector4 v;
    Lerp(a, b, t, v);
    return v;
}

float Vector4::Dot(const Vector4& v) const                                { return Dot(*this, v); }
float Vector4::DistanceSquared(const Vector4& v) const                    { return DistanceSquared(*this, v); }
float Vector4::Distance(const Vector4& v) const                           { return Distance(*this, v); }

Vector4 Vector4::Lerp(const Vector4& v, float t) const {
    Vector4 temp;
    Vector4::Lerp(*this, v, t, temp);
    return temp;
}

#undef IDX_X
#undef IDX_Y
#undef IDX_Z
#undef IDX_W


XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4Operators.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

const Vector4& Matrix4x4::operator [](int i) const {
    return r[i];
}

Vector4& Matrix4x4::operator [](int i) {
    return r[i];
}

const float& Matrix4x4::operator ()(int r, int c) const {
    return this->r[r][c];
}

float& Matrix4x4::operator ()(int r, int c) {
    return this->r[r][c];
}

const Vector4& Matrix4x4::GetRow(int i) const {
    return r[i];
}

Vector4 Matrix4x4::GetColumn(int i) const {
    return Vector4(r[0][i], r[1][i], r[2][i], r[3][i]);
}

Matrix4x4 Matrix4x4::operator ~() const {
    auto m = *this;
    return m.Transpose();
}

const Matrix4x4& Matrix4x4::operator += (const Matrix4x4& m) {
    r[0] += m[0];
    r[1] += m[1];
    r[2] += m[2];
    r[3] += m[3];
    return *this;
}

const Matrix4x4& Matrix4x4::operator -= (const Matrix4x4& m) {
    r[0] -= m[0];
    r[1] -= m[1];
    r[2] -= m[2];
    r[3] -= m[3];
    return *this;
}

const Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& m) {
    auto t = m.Transposed();
    return (*this) = Matrix4x4(
        (r[0] * t[0]).Sum(), (r[0] * t[1]).Sum(), (r[0] * t[2]).Sum(), (r[0] * t[3]).Sum(),
        (r[1] * t[0]).Sum(), (r[1] * t[1]).Sum(), (r[1] * t[2]).Sum(), (r[1] * t[3]).Sum(),
        (r[2] * t[0]).Sum(), (r[2] * t[1]).Sum(), (r[2] * t[2]).Sum(), (r[2] * t[3]).Sum(),
        (r[3] * t[0]).Sum(), (r[3] * t[1]).Sum(), (r[3] * t[2]).Sum(), (r[3] * t[3]).Sum()
    );
}

Matrix4x4 Matrix4x4::operator + (const Matrix4x4& m) const { return Matrix4x4(*this) += m; }
Matrix4x4 Matrix4x4::operator - (const Matrix4x4& m) const { return Matrix4x4(*this) -= m; }
Matrix4x4 Matrix4x4::operator * (const Matrix4x4& m) const { return Matrix4x4(*this) *= m; }

Vector4 Matrix4x4::operator * (const Vector4& v) const {
    return Vector4((r[0] * v).Sum(), (r[1] * v).Sum(), (r[2] * v).Sum(), (r[3] * v).Sum());
}

Vector3 Matrix4x4::operator * (const Vector3& v) const {
    return Vector3((r[0] * v).Sum(), (r[1] * v).Sum(), (r[2] * v).Sum());
}

XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include Matrix4x4Methods.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

#if defined(_XO_ASSERT_MSG)
_XOMATH_INTERNAL_MACRO_WARNING
#else
#   define _XO_ASSERT_MSG(msg) "xo-math Matrix4x4" msg
#endif

XOMATH_BEGIN_XO_NS();

Matrix4x4::Matrix4x4() {
}

Matrix4x4::Matrix4x4(float m)
#if XO_SSE
{ 
    r[0].m = r[1].m = r[2].m = r[3].m = _mm_set_ps1(m);
}
#else
{
	r[0] = Vector4(m);
	r[1] = Vector4(m);
	r[2] = Vector4(m);
	r[3] = Vector4(m);
}
#endif

Matrix4x4::Matrix4x4(float a0, float b0, float c0, float d0, float a1, float b1, float c1, float d1, float a2, float b2, float c2, float d2, float a3, float b3, float c3, float d3)
{
    r[0] = Vector4(a0, b0, c0, d0);
    r[1] = Vector4(a1, b1, c1, d1);
    r[2] = Vector4(a2, b2, c2, d2);
    r[3] = Vector4(a3, b3, c3, d3);
}

// TODO: couldn't this be faster with just a single copy?
Matrix4x4::Matrix4x4(const Matrix4x4& m)
{
	r[0] = m.r[0];
	r[1] = m.r[1];
	r[2] = m.r[2];
	r[3] = m.r[3];
}

Matrix4x4::Matrix4x4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3)
{
	r[0] = r0;
	r[1] = r1;
	r[2] = r2;
	r[3] = r3;
}

Matrix4x4::Matrix4x4(const Vector3& r0, const Vector3& r1, const Vector3& r2)
{
	r[0] = Vector4(r0);
	r[1] = Vector4(r1);
	r[2] = Vector4(r2);
	r[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4::Matrix4x4(const class Quaternion& q) {
    Vector4* v4 = (Vector4*)&q;
    Vector4 q2 = *v4 + *v4;

    Vector4 qq2 = (*v4) * q2;
    Vector4 wq2 = q2 * q.w;

    float xy2 = q.x * q2.y;
    float xz2 = q.x * q2.z;
    float yz2 = q.y * q2.z;

    r[0] = Vector4( 1.0f - qq2.y - qq2.z,  xy2 + wq2.z,            xz2 - wq2.y,          0.0f);
    r[1] = Vector4( xy2 - wq2.z,           1.0f - qq2.x - qq2.z,   yz2 + wq2.x,          0.0f);
    r[2] = Vector4( xz2 + wq2.y,           yz2 - wq2.x,            1.0f - qq2.x - qq2.y, 0.0f);
    r[3] = Vector4::UnitW;
}

const Matrix4x4& Matrix4x4::Transpose() {
#if XO_SSE
    _MM_TRANSPOSE4_PS(r[0].m, r[1].m, r[2].m, r[3].m);
#else
    float t;
#   define _XO_TRANSPOSE_SWAP(i,j) t = r[i][j]; r[i][j] = r[j][i]; r[j][i] = t;
    // The number of swaps should be:
    //      (((n*n)-n)/2) = 6
    //      (((4*4)-4)/2) = 6
    //      (((all elements)-identity)/(two elements processed per swap)) = 6
    _XO_TRANSPOSE_SWAP(0, 1);
    _XO_TRANSPOSE_SWAP(0, 2);
    _XO_TRANSPOSE_SWAP(0, 3);
    _XO_TRANSPOSE_SWAP(1, 2);
    _XO_TRANSPOSE_SWAP(1, 3);
    _XO_TRANSPOSE_SWAP(2, 3);
#   undef _XO_TRANSPOSE_SWAP
#endif
    return *this;
}

Matrix4x4 Matrix4x4::Transposed() const {
    Matrix4x4 m(*this);
    return m.Transpose();
}

const Matrix4x4& Matrix4x4::Transform(Vector3& v) const {
    v = (*this) * Vector4(v);
    return *this;
}

const Matrix4x4& Matrix4x4::Transform(Vector4& v) const {
    v = (*this) * v;
    return *this;
}

void Matrix4x4::Scale(float xyz, Matrix4x4& m) {
    // Also valid but requires division or recip:
    // 1 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // 0 0 0 1/xyz
    m = Matrix4x4(
            xyz,  0.0f, 0.0f, 0.0f,
            0.0f, xyz,  0.0f, 0.0f,
            0.0f, 0.0f, xyz,  0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
}
 
void Matrix4x4::Scale(float x, float y, float z, Matrix4x4& m) {
    m = Matrix4x4(
            x,    0.0f, 0.0f, 0.0f,
            0.0f, y,    0.0f, 0.0f,
            0.0f, 0.0f, z,    0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
}

void Matrix4x4::Scale(const Vector3& v, Matrix4x4& m) {
    m = Matrix4x4(
            v.x,  0.0f, 0.0f, 0.0f,
            0.0f, v.y,  0.0f, 0.0f,
            0.0f, 0.0f, v.z,  0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
}
 
void Matrix4x4::Translation(float x, float y, float z, Matrix4x4& m) {
   m = Matrix4x4(
            1.0f, 0.0f, 0.0f, x   ,
            0.0f, 1.0f, 0.0f, y   ,
            0.0f, 0.0f, 1.0f, z   ,
            0.0f, 0.0f, 0.0f, 1.0f
        );
}

void Matrix4x4::Translation(const Vector3& v, Matrix4x4& m) {
    m = Matrix4x4(
            1.0f, 0.0f, 0.0f, v.x ,
            0.0f, 1.0f, 0.0f, v.y ,
            0.0f, 0.0f, 1.0f, v.z ,
            0.0f, 0.0f, 0.0f, 1.0f
        );
}

void Matrix4x4::RotationXRadians(float radians, Matrix4x4& m) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    m = Matrix4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosr,-sinr, 0.0f,
            0.0f, sinr, cosr, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    
}
 
void Matrix4x4::RotationYRadians(float radians, Matrix4x4& m) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    m = Matrix4x4(
            cosr, 0.0f,-sinr, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sinr, 0.0f, cosr, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
}

void Matrix4x4::RotationZRadians(float radians, Matrix4x4& m) {
    float cosr = Cos(radians);
    float sinr = Sin(radians);
    m = Matrix4x4(
            cosr,-sinr, 0.0f, 0.0f,
            sinr, cosr, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
}

void Matrix4x4::RotationRadians(float x, float y, float z, Matrix4x4& m) {
    Matrix4x4 mx, mz;
    RotationXRadians(x, mx);
    RotationYRadians(y, m);
    RotationZRadians(z, mz);
    m *= mx * mz;
}

void Matrix4x4::RotationRadians(const Vector3& v, Matrix4x4& m) {
    Matrix4x4 mx, mz;
    RotationXRadians(v.x, mx);
    RotationYRadians(v.y, m);
    RotationZRadians(v.z, mz);
    m *= mx * mz;
}

void Matrix4x4::AxisAngleRadians(const Vector3& a, float radians, Matrix4x4& m) {
    float c = Cos(radians);
    float s = Sin(radians);
    float t = 1.0f - c;
    const float& x = a.x;
    const float& y = a.y;
    const float& z = a.z;
    m = Matrix4x4(
             t*x*x+c  ,    t*x*y-z*s,  t*x*z+y*s,  0.0f,
             t*x*y+z*s,    t*y*y+c  ,  t*y*z-x*s,  0.0f,
             t*x*z-y*s,    t*y*z+x*s,  t*z*z+c  ,  0.0f,
             0.0f,         0.0f,       0.0f,       1.0f
        );
}

void Matrix4x4::RotationXDegrees(float degrees, Matrix4x4& m) {
    RotationXRadians(degrees * Deg2Rad, m);
}

void Matrix4x4::RotationYDegrees(float degrees, Matrix4x4& m) {
    RotationYRadians(degrees * Deg2Rad, m);
}

void Matrix4x4::RotationZDegrees(float degrees, Matrix4x4& m) {
    RotationZRadians(degrees * Deg2Rad, m);
}

void Matrix4x4::RotationDegrees(float x, float y, float z, Matrix4x4& m) {
    Matrix4x4 m0, m1, m2;
    RotationXDegrees(x, m0);
    RotationYDegrees(y, m1);
    RotationZDegrees(z, m2);
    m = m0 * m1 * m2;
}

void Matrix4x4::RotationDegrees(const Vector3& v, Matrix4x4& m) {
    Matrix4x4 m0, m1, m2;
    RotationXDegrees(v.x, m0);
    RotationYDegrees(v.y, m1);
    RotationZDegrees(v.z, m2);
    m = m0 * m1 * m2;
}


void Matrix4x4::AxisAngleDegrees(const Vector3& a, float degrees, Matrix4x4& m) {
    AxisAngleRadians(a, degrees * Deg2Rad, m);
}

void Matrix4x4::OrthographicProjection(float w, float h, float n, float f, Matrix4x4& m) {
    XO_ASSERT(w != 0.0f, _XO_ASSERT_MSG("::OrthographicProjection Width (w) should not be zero."));
    XO_ASSERT(h != 0.0f, _XO_ASSERT_MSG("::OrthographicProjection Height (h) should not be zero."));
    m = Matrix4x4(
            1.0f/w,    0.0f,       0.0f,    0.0f,
            0.0f,      1.0f/h,     0.0f,    0.0f,
            0.0f,      0.0f,       f-n,     0.0f,
            0.0f,      0.0f,       n*(f-n), 1.0f
        );
}
 
void Matrix4x4::PerspectiveProjectionRadians(float fovx, float fovy, float n, float f, Matrix4x4& m) {
    XO_ASSERT(n != f, _XO_ASSERT_MSG("::PerspectiveProjectionRadians Near (n) and far (f) values should not be equal."));
    m = Matrix4x4(
            ATan(fovx/2.0f),   0.0f,               0.0f,               0.0f,
            0.0f,              ATan(fovy/2.0f),    0.0f,               0.0f,
            0.0f,              0.0f,               f/(f-n),            1.0f,
            0.0f,              0.0f,               -n*(f/-n),          1.0f
        );
}

void Matrix4x4::PerspectiveProjectionDegrees(float fovx, float fovy, float near, float far, Matrix4x4& outMatrix) {
    Matrix4x4::PerspectiveProjectionRadians(fovx * Deg2Rad, fovy * Deg2Rad, near, far, outMatrix);
}

void Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Matrix4x4& m) {
    Vector3 zAxis = (to - from).Normalized();
    Vector3 xAxis = Vector3::Cross(up, zAxis).Normalized();
    Vector3 yAxis = Vector3::Cross(zAxis, xAxis);
    m = Matrix4x4(
            xAxis.x,           yAxis.x,            zAxis.x,            0.0f,
            xAxis.y,           yAxis.y,            zAxis.y,            0.0f,
            xAxis.z,           yAxis.z,            zAxis.z,            0.0f,
            -xAxis.Dot(from),  -yAxis.Dot(from),   -zAxis.Dot(from),   1.0f
        );
}

void Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to, Matrix4x4& m) {
    LookAtFromPosition(from, to, Vector3::Up, m);
}

void Matrix4x4::LookAtFromDirection(const Vector3& direction, const Vector3& up, Matrix4x4& m) {
    Vector3 zAxis = direction.Normalized();
    Vector3 xAxis = Vector3::Cross(up, zAxis).Normalized();
    Vector3 yAxis = Vector3::Cross(zAxis, xAxis);
    m = Matrix4x4(
            xAxis.x,           yAxis.x,            zAxis.x,            0.0f,
            xAxis.y,           yAxis.y,            zAxis.y,            0.0f,
            xAxis.z,           yAxis.z,            zAxis.z,            0.0f,
            0.0f,              0.0f,               0.0f,               1.0f
        );
}

void Matrix4x4::LookAtFromDirection(const Vector3& direction, Matrix4x4& m) {
    LookAtFromDirection(direction, Vector3::Up, m);
}

Matrix4x4 Matrix4x4::Scale(float xyz) {
    Matrix4x4 m;
    Scale(xyz, m);
    return m;
}
Matrix4x4 Matrix4x4::Scale(float x, float y, float z) {
    Matrix4x4 m;
    Scale(x, y, z, m);
    return m;
}
Matrix4x4 Matrix4x4::Scale(const Vector3& v) {
    Matrix4x4 m;
    Scale(v, m);
    return m;
}

Matrix4x4 Matrix4x4::Translation(float x, float y, float z) {
    Matrix4x4 m;
    Translation(x, y, z, m);
    return m;
}
Matrix4x4 Matrix4x4::Translation(const Vector3& v) {
    Matrix4x4 m;
    Translation(v, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationXRadians(float radians) {
    Matrix4x4 m;
    RotationXRadians(radians, m);
    return m;
}
Matrix4x4 Matrix4x4::RotationYRadians(float radians) {
    Matrix4x4 m;
    RotationYRadians(radians, m);
    return m;
}
Matrix4x4 Matrix4x4::RotationZRadians(float radians) {
    Matrix4x4 m;
    RotationZRadians(radians, m);
    return m;
}
Matrix4x4 Matrix4x4::RotationRadians(float x, float y, float z) {
    Matrix4x4 m;
    RotationRadians(x, y, z, m);
    return m;
}
Matrix4x4 Matrix4x4::RotationRadians(const Vector3& v) {
    Matrix4x4 m;
    RotationRadians(v, m);
    return m;
}
Matrix4x4 Matrix4x4::AxisAngleRadians(const Vector3& axis, float radians) {
    Matrix4x4 m;
    AxisAngleRadians(axis, radians, m);
    return m;
}

Matrix4x4 Matrix4x4::RotationXDegrees(float degrees) {
    Matrix4x4 m;
    RotationXDegrees(degrees, m);
    return m;
}
Matrix4x4 Matrix4x4::RotationYDegrees(float degrees) {
    Matrix4x4 m;
    RotationYDegrees(degrees, m);
    return m;
}
Matrix4x4 Matrix4x4::RotationZDegrees(float degrees) {
    Matrix4x4 m;
    RotationZDegrees(degrees, m);
    return m;
}
Matrix4x4 Matrix4x4::RotationDegrees(float x, float y, float z) {
    Matrix4x4 m;
    RotationDegrees(x, y, z, m);
    return m;
}
Matrix4x4 Matrix4x4::RotationDegrees(const Vector3& v) {
    Matrix4x4 m;
    RotationDegrees(v, m);
    return m;
}
Matrix4x4 Matrix4x4::AxisAngleDegrees(const Vector3& axis, float degrees) {
    Matrix4x4 m;
    AxisAngleDegrees(axis, degrees, m);
    return m;
}

Matrix4x4 Matrix4x4::OrthographicProjection(float w, float h, float n, float f) {
    Matrix4x4 m;
    OrthographicProjection(w, h, n, f, m);
    return m;
}

Matrix4x4 Matrix4x4::PerspectiveProjectionRadians(float fovx, float fovy, float n, float f) {
    Matrix4x4 m;
    PerspectiveProjectionRadians(fovx, fovy, n, f, m);
    return m;
}

Matrix4x4 Matrix4x4::PerspectiveProjectionDegrees(float fovx, float fovy, float n, float f) {
    Matrix4x4 m;
    PerspectiveProjectionDegrees(fovx, fovy, n, f, m);
    return m;
}

Matrix4x4 Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up) {
    Matrix4x4 m;
    LookAtFromPosition(from, to, up, m);
    return m;
}
Matrix4x4 Matrix4x4::LookAtFromPosition(const Vector3& from, const Vector3& to) {
    Matrix4x4 m;
    LookAtFromPosition(from, to, m);
    return m;
}
Matrix4x4 Matrix4x4::LookAtFromDirection(const Vector3& direction, const Vector3& up) {
    Matrix4x4 m;
    LookAtFromDirection(direction, up, m);
    return m;
}
Matrix4x4 Matrix4x4::LookAtFromDirection(const Vector3& direction) {
    Matrix4x4 m;
    LookAtFromDirection(direction, m);
    return m;
}

XOMATH_END_XO_NS();

#undef _XO_ASSERT_MSG

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include QuaternionOperators.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

float& Quaternion::operator [](int i) { 
  return f[i]; 
}

const float& Quaternion::operator [](int i) const { 
  return f[i]; 
}

const Quaternion& Quaternion::operator *= (const Quaternion& q) {
    // TODO: see if there's a cute intrinsic way to do this.
    _XO_ASSIGN_QUAT(
      w * q.x + x * q.w + y * q.z - z * q.y,
      w * q.y - x * q.z + y * q.w + z * q.x,
      w * q.z + x * q.y - y * q.x + z * q.w,
      w * q.w - x * q.x - y * q.y - z * q.z);
  return *this;
}

Quaternion Quaternion::operator * (const Quaternion& q) const {
  return Quaternion(*this) *= q;
}

bool Quaternion::operator == (const Quaternion& q) const {
#   if XO_SSE
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(q.m, m)), sse::Epsilon)) & 15) == 15;
#   else
    return 
        CloseEnough(x, q.x, Epsilon) && 
        CloseEnough(y, q.y, Epsilon) && 
        CloseEnough(z, q.z, Epsilon) &&
        CloseEnough(w, q.w, Epsilon);
#   endif
}

bool Quaternion::operator != (const Quaternion& q) const {
  return !((*this) == q);
}

XOMATH_END_XO_NS();

#endif

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include QuaternionMethods.h directly. Include xo-math.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

namespace xo_internal {
    _XOINL float QuaternionSquareSum(const Quaternion& q) {
#if XO_SSE
        __m128 square = _mm_mul_ps(q.m, q.m);
        square = _mm_hadd_ps(square, square);
        square = _mm_hadd_ps(square, square);
        return _mm_cvtss_f32(square);
#else
        return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
#endif
    }
}

Quaternion::Quaternion() {
}

Quaternion::Quaternion(const Matrix4x4& mat) {
    Vector3 xAxis(mat[0]);
    Vector3 yAxis(mat[1]);
    Vector3 zAxis(mat[2]);

    Vector3 scale(xAxis.Magnitude(), yAxis.Magnitude(), zAxis.Magnitude());

    // don't use close enough, skip the abs since we're all positive value.
    if(scale.x <= FloatEpsilon || scale.y <= FloatEpsilon || scale.z <= FloatEpsilon) {
#if XO_SSE

#else
        w = 1.0f;
        x = y = z = 0.0f;
#endif
        return; // too close.
    }

#if XO_SSE
#   if defined(XO_NO_INVERSE_DIVISION)
    Vector3 recipScale = Vector3(_mm_div_ps(Vector4::One.m, scale.m));
#   else
    Vector3 recipScale = Vector3(_mm_rcp_ps(scale.m));
#   endif
#else
    Vector3 recipScale = Vector3(1.0f/scale.x, 1.0f/scale.y, 1.0f/scale.z);
#endif
    xAxis *= recipScale.x;
    yAxis *= recipScale.y;
    zAxis *= recipScale.z;

    // Now calculate the rotation from the resulting matrix (axes).
    float trace = xAxis.x + yAxis.y + zAxis.z + 1.0f;

    if (trace > 1.0f)
    {
        float s = 0.5f / Sqrt(trace);
        _XO_ASSIGN_QUAT(
            0.25f / s,
            (yAxis.z- zAxis.y) * s,
            (xAxis.y - yAxis.x) * s,
            (zAxis.x - xAxis.z) * s);
    }
    else
    {
        if (xAxis.x > yAxis.y && xAxis.x > zAxis.z)
        {
            float s = 0.5f / Sqrt(1.0f + xAxis.x - yAxis.y - zAxis.z);
            _XO_ASSIGN_QUAT(
                (yAxis.z - zAxis.y) * s,
                0.25f / s,
                (zAxis.x + xAxis.z) * s,
                (yAxis.x + xAxis.y) * s);
        }
        else if (yAxis.y > zAxis.z)
        {
            float s = 0.5f / Sqrt(1.0f + yAxis.y - xAxis.x - zAxis.z);
            _XO_ASSIGN_QUAT(
                (zAxis.x - xAxis.z) * s,
                (yAxis.x + xAxis.y) * s,
                (zAxis.y + yAxis.z) * s,
                0.25f / s);
        }
        else
        {
            float s = 0.5f / Sqrt(1.0f + zAxis.z - xAxis.x - yAxis.y );
            _XO_ASSIGN_QUAT(
                (xAxis.y - yAxis.x ) * s,
                (zAxis.x + xAxis.z ) * s,
                0.25f / s,
                (zAxis.y + yAxis.z ) * s);
        }
    }
}

Quaternion::Quaternion(float x, float y, float z, float w)  :
#if XO_SSE
    m(_mm_set_ps(w, z, y, x))
#else
    x(x), y(y), z(z), w(w)
#endif
{
}

Quaternion Quaternion::Inverse() const {
    return Quaternion(*this).MakeInverse();
}

const Quaternion& Quaternion::MakeInverse() {
    float magnitude = xo_internal::QuaternionSquareSum(*this);
    
    if (CloseEnough(magnitude, 1.0f, Epsilon)) {
        return MakeConjugate();
    }
    if (CloseEnough(magnitude, 0.0f, Epsilon)) {
        return *this;
    }

    MakeConjugate();
    (*(Vector4*)this) /= magnitude;
    return *this;
}

Quaternion Quaternion::Normalized() const {
    return Quaternion(*this).Normalize();
}

const Quaternion& Quaternion::Normalize() {
    float magnitude = xo_internal::QuaternionSquareSum(*this);
    if (CloseEnough(magnitude, 1.0f, Epsilon)) {
        return *this;
    }

    magnitude = Sqrt(magnitude);
    if (CloseEnough(magnitude, 0.0f, Epsilon)) {
        return *this;
    }

    (*(Vector4*)this) /= magnitude;
    return *this;
}

Quaternion Quaternion::Conjugate() const {
    return Quaternion(*this).MakeConjugate();
}

const Quaternion& Quaternion::MakeConjugate() {
    _XO_ASSIGN_QUAT(w, -x, -y, -z);
    return *this;
}

void Quaternion::GetAxisAngleRadians(Vector3& axis, float& radians) const {
    Quaternion q = Normalized();

#if XO_SSE
    axis.m = q.m;
#else
    axis.x = q.x;
    axis.y = q.y;
    axis.z = q.z;
    axis.Normalize();
#endif
    radians = (2.0f * ACos(q.w));
}

void Quaternion::RotationRadians(float x, float y, float z, Quaternion& outQuat) {
    RotationRadians(Vector3(x, y, z), outQuat);
}

void Quaternion::RotationRadians(const Vector3& v, Quaternion& outQuat) {
    Vector3 hv = v * 0.5f;

#if XO_SSE && defined(__INTEL_COMPILER)
    Vector3 vs(_mm_sin_ps(hv));
    Vector3 vc(_mm_cos_ps(hv));
#else
    // agner fog has a portable vector trig.... see if the licence is usable.
    Vector3 vs(Sin(hv.x), Sin(hv.y), Sin(hv.z));
    Vector3 vc(Cos(hv.x), Cos(hv.y), Cos(hv.z));
#endif
    _XO_ASSIGN_QUAT_Q(outQuat,
        vc.x * vc.y * vc.z + vs.x * vs.y * vs.z,
        vs.x * vc.y * vc.z - vc.x * vs.y * vs.z,
        vc.x * vs.y * vc.z + vs.x * vc.y * vs.z,
        vc.x * vc.y * vs.z - vs.x * vs.y * vc.z);
}

void Quaternion::AxisAngleRadians(const Vector3& axis, float radians, Quaternion& outQuat) {
    float hr = radians * 0.5f;
    float sr = Sin(hr);

    Vector3 n = axis.Normalized();
    n *= sr;
    _XO_ASSIGN_QUAT_Q(outQuat, Cos(radians), n.x, n.y, n.z);
}

void Quaternion::LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up, Quaternion& outQuat) {
    LookAtFromDirection(to-from, up, outQuat);
}

void Quaternion::LookAtFromPosition(const Vector3& from, const Vector3& to, Quaternion& outQuat) {
    LookAtFromPosition(from, to, Vector3::Up, outQuat);
}

void Quaternion::LookAtFromDirection(const Vector3& direction, const Vector3& up, Quaternion& outQuat) {
    // Todo
}

void Quaternion::LookAtFromDirection(const Vector3& direction, Quaternion& outQuat) {
    LookAtFromDirection(direction, Vector3::Up, outQuat);
}

void Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat) {
    //      The folowing copyright and licence applies to the contents of this Quaternion::Slerp method

    //      Copyright 2013 BlackBerry Inc.
    //      Licensed under the Apache License, Version 2.0 (the "License");
    //      you may not use this file except in compliance with the License.
    //      You may obtain a copy of the License at
    //      http://www.apache.org/licenses/LICENSE-2.0
    //      Unless required by applicable law or agreed to in writing, software
    //      distributed under the License is distributed on an "AS IS" BASIS,
    //      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    //      See the License for the specific language governing permissions and
    //      limitations under the License.

    //      Original file from GamePlay3D: http://gameplay3d.org

    //      Modified by Jared Thomson in 2016:
    //        - removed assertions
    //        - made compatable with xo-math

    // Fast slerp implementation by kwhatmough:
    // It contains no division operations, no trig, no inverse trig
    // and no sqrt. Not only does this code tolerate small constraint
    // errors in the input quaternions, it actually corrects for them.
    if (CloseEnough(t, 0.0f)) {
        outQuat = a;
        return;
    }
    else if (CloseEnough(t, 1.0f)) {
        outQuat = b;
        return;
    }
    else if (a == b) {
        outQuat = a;
    }
    else {
        float halfY, alpha, beta;
        float u, f1, f2a, f2b;
        float ratio1, ratio2;
        float halfSecHalfTheta, versHalfTheta;
        float sqNotU, sqU;

        Vector4 * va = (Vector4*)&a;
        Vector4 * vb = (Vector4*)&b;

        float cosTheta = ((*va) * (*vb)).Sum();

        // As usual in all slerp implementations, we fold theta.
        alpha = cosTheta >= 0 ? 1.0f : -1.0f;
        halfY = 1.0f + alpha * cosTheta;

        // Here we bisect the interval, so we need to fold t as well.
        f2b = t - 0.5f;
        u = f2b >= 0 ? f2b : -f2b;
        f2a = u - f2b;
        f2b += u;
        u += u;
        f1 = 1.0f - u;

        // One iteration of Newton to get 1-cos(theta / 2) to good accuracy.
        halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
        halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
        versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

        // Evaluate series expansions of the coefficients.
        sqNotU = f1 * f1;
        ratio2 = 0.0000440917108f * versHalfTheta;
        ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
        ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
        ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
        ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

        sqU = u * u;
        ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
        ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
        ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
        ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

        // Perform the bisection and resolve the folding done earlier.
        f1 *= ratio1 * halfSecHalfTheta;
        f2a *= ratio2;
        f2b *= ratio2;
        alpha *= f1 + f2a;
        beta = f1 + f2b;

        // Apply final coefficients to a and b as usual.
        float w = alpha * a.w + beta * b.w;
        float x = alpha * a.x + beta * b.x;
        float y = alpha * a.y + beta * b.y;
        float z = alpha * a.z + beta * b.z;

        // This final adjustment to the quaternion's length corrects for
        // any small constraint error in the inputs q1 and q2 But as you
        // can see, it comes at the cost of 9 additional multiplication
        // operations. If this error-correcting feature is not required,
        // the following code may be removed.
        f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
        _XO_ASSIGN_QUAT_Q(outQuat, w * f1, x * f1, y * f1, z * f1);
    }
}

void Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t, Quaternion& outQuat) {
    if (CloseEnough(t, 0.0f, Epsilon)) {
        outQuat = a;
    }
    else if (CloseEnough(t, 1.0f, Epsilon)) {
        outQuat = b;
    }
    else {
        Vector4* vq = (Vector4*)&outQuat;
        Vector4* va = (Vector4*)&a;
        Vector4* vb = (Vector4*)&b;
        // Todo: give the vectors the same static 'outQuat' type interface, then just use the Vector4::Lerp for this lerp
        (*vq) = *va + ((*vb - *va) * t);
    }
}

Quaternion Quaternion::RotationRadians(float x, float y, float z) {
    Quaternion q;
    RotationRadians(x, y, z, q);
    return q;
}

Quaternion Quaternion::RotationRadians(const Vector3& v) {
    Quaternion q;
    RotationRadians(v, q);
    return q;
}

Quaternion Quaternion::AxisAngleRadians(const Vector3& axis, float radians) {
    Quaternion q;
    AxisAngleRadians(axis, radians, q);
    return q;
}

Quaternion Quaternion::LookAtFromPosition(const Vector3& from, const Vector3& to, const Vector3& up) {
    Quaternion q;
    LookAtFromPosition(from, to, up, q);
    return q;
}

Quaternion Quaternion::LookAtFromPosition(const Vector3& from, const Vector3& to) {
    Quaternion q;
    LookAtFromPosition(from, to, q);
    return q;
}

Quaternion Quaternion::LookAtFromDirection(const Vector3& direction, const Vector3& up) {
    Quaternion q;
    LookAtFromDirection(direction, up, q);
    return q;
}

Quaternion Quaternion::LookAtFromDirection(const Vector3& direction) {
    Quaternion q;
    LookAtFromDirection(direction, q);
    return q;
}

Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t) {
    Quaternion q;
    Slerp(a, b, t, q);
    return q;
}

Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t) {
    Quaternion q;
    Lerp(a, b, t, q);
    return q;
}

XOMATH_END_XO_NS();

#endif


#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include SSE.h directly. Include xo-math.h.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS();

_XOINL Vector2 Abs(const Vector2& v) {
    return Vector2(Abs(v.x), Abs(v.y));
}

_XOINL Vector3 Abs(const Vector3& v) {
    return (sse::Abs(v.m));
}

_XOINL Vector4 Abs(const Vector4& v) {
    return Vector4(sse::Abs(v.m));
}

#if XO_SSE

namespace sse {

    // The control of MXCSR usage is inspired by Agner Fog's use of them in vectorclasses.
    // vectorclasses uses them to optionally speed up subnormal operations.
    // To achieve this in xomath, call the following once per thread where xo-math is used:
    //      sse::UpdateControlWord();       // updates the thread-local state.
    //      sse::SetDenormalsAreZero(true); // force all denormal values to 0
    //      sse::SetFlushToZero(true);      // underflowing operations produce 0
    // Note: this will only produce speed gains where subnormal values are likely to occur.
    // See http://wm.ite.pl/articles/sse-penalties-of-errors.html for more details.
    namespace mxcsr {
        // Flags that are set on the CPU if an exception had occured.
        // They will remain set until manually unset.
        enum class Flags {
            InvalidOperation                = (1 << 0),
            Denormal                        = (1 << 1),
            DivideByZero                    = (1 << 2),
            Overflow                        = (1 << 3),
            Underflow                       = (1 << 4),
            Precision                       = (1 << 5),
        };

        enum class DAZ {
            DenormalsAreZero                = (1 << 6),
        };

        enum class Masks {
            InvalidOperation                = (1 << 7),
            Denormal                        = (1 << 8),
            DivideByZero                    = (1 << 9),
            Overflow                        = (1 << 10),
            Underflow                       = (1 << 11),
            Precision                       = (1 << 12)
        };

        enum class Rounding {
            Nearest                         = 0,
            Negative                        = (1 << 13),
            Positive                        = (1 << 14),
            Zero                            = (1 << 13) | (1 << 14),
            
            Bits                            = (1 << 13) | (1 << 14)
        };

        enum class FZ {
            FlushToZero                     = (1 << 15)
        };
    }

    bool GetControlMask(mxcsr::Masks mask, bool withUpdate = false);
    bool GetControlMask(unsigned mask, bool withUpdate = false);
    bool GetDenormalExceptionMask(bool withUpdate = false);
    bool GetDivideByZeroExceptionMask(bool withUpdate = false);
    bool GetInvalidOperationExceptionMask(bool withUpdate = false);
    bool GetOverflowExceptionMask(bool withUpdate = false);
    bool GetPrecisionExceptionMask(bool withUpdate = false);
    bool GetUnderflowExceptionMask(bool withUpdate = false);

    bool HasControlFlagBeenSet(mxcsr::Flags flags, bool withUpdate = false, bool thenFlush = false);
    bool HasControlFlagBeenSet(unsigned flags, bool withUpdate = false, bool thenFlush = false);
    bool HasDenormalExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasDenormalsAreZeroSet(bool withUpdate = false);
    bool HasDivideByZeroExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasFlushToZeroSet(bool withUpdate = false);
    bool HasInvalidOperationExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasOverflowExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasPrecisionExceptionOccured(bool withUpdate = false, bool thenFlush = false);
    bool HasUnderflowExceptionOccured(bool withUpdate = false, bool thenFlush = false);

    mxcsr::Rounding GetRoundingMode(bool withUpdate = false);
    void GetAllMXCSRInfo(std::ostream& os, bool withUpdate = false);

    void RemoveControlWord(unsigned control);

    void SetControlMask(mxcsr::Masks mask, bool value, bool withUpdate = false);
    void SetControlMask(unsigned mask, bool value, bool withUpdate = false);
    void SetControlWord(unsigned control);
    void SetControlWordAddative(unsigned control);
    void SetDenormalExceptionMask(bool value, bool withUpdate = false);
    void SetDenormalsAreZero(bool value, bool withUpdate = false);
    void SetDivideByZeroExceptionMask(bool value, bool withUpdate = false);
    void SetFlushToZero(bool value, bool withUpdate = false);
    void SetInvalidOperationExceptionMask(bool value, bool withUpdate = false);
    void SetOverflowExceptionMask(bool value, bool withUpdate = false);
    void SetPrecisionExceptionMask(bool value, bool withUpdate = false);
    void SetRoundingMode(mxcsr::Rounding mode, bool withUpdate = false);
    void SetRoundingMode(unsigned mode, bool withUpdate = false);
    void SetUnderflowExceptionMask(bool value, bool withUpdate = false);

    void ThrowAllExceptions(bool withUpdate = false);
    void ThrowNoExceptions(bool withUpdate = false);
    void UpdateControlWord();
}
#endif

XOMATH_END_XO_NS();

#endif // XOMATH_INTERNAL


////////////////////////////////////////////////////////////////////////// Remove internal macros

#if defined(XO_REDEFINABLE)
// For testing it's helpful to change settings and re-include xo-math with another XO_CUSTOM_NS defined as well.
#   undef XO_MATH_H
#endif

// don't undef the namespace macros inside xo-math cpp files.
#if !defined(_XO_MATH_OBJ)
#   undef XOMATH_BEGIN_XO_NS
#   undef XOMATH_END_XO_NS
#endif

#if !defined(XO_EXPORT_ALL)
#   undef _XOMATH_INTERNAL_MACRO_WARNING

#   undef _XOCONSTEXPR
#   undef _XOINL
#   undef _XOTLS

#   undef _XO_TLS_ENGINE
#   undef _XO_TLS_DISTRIBUTION

#   undef _XO_OVERLOAD_NEW_DELETE

#   undef _XO_MIN
#   undef _XO_MAX

#   undef _XO_ASSIGN_QUAT
#   undef _XO_ASSIGN_QUAT_Q

#   undef XOMATH_INTERNAL
#endif

////////////////////////////////////////////////////////////////////////// Add external macros

// version kinds:
// x: experimental, do not use for production.
// a: alpha, for specific feature testing. Contains untested features not in the last major revision.
// b: beta, for broad user testing.
// r: release, all features broadly tested in various applications.
// p: patch release, contains fixes for a release version.

#define XO_MATH_VERSION_DATE "Fall 2016"
#define XO_MATH_VERSION_MAJOR 0
#define XO_MATH_VERSION_KIND "x"
#define XO_MATH_VERSION_MINOR 3
#define XO_MATH_VERSION_SUB 0
#define XO_MATH_VERSION_STR _XO_MATH_STRINGIFY(XO_MATH_VERSION_MAJOR) "." _XO_MATH_STRINGIFY(XO_MATH_VERSION_MINOR) "." XO_MATH_VERSION_KIND _XO_MATH_STRINGIFY(XO_MATH_VERSION_SUB)
#define XO_MATH_VERSION (XO_MATH_VERSION_MAJOR*10000) + (XO_MATH_VERSION_MINOR*1000) + (XO_MATH_VERSION_SUB*100)

#define XO_MATH_VERSION_TXT "xo-math version " XO_MATH_VERSION_STR " " XO_MATH_VERSION_DATE "."

#if defined(_MSC_VER)
#   define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with msvc " _XO_MATH_STRINGIFY(_MSC_VER) ", supporting simd: " XO_MATH_HIGHEST_SIMD "."
#   pragma message(XO_MATH_COMPILER_INFO)
#elif defined(__clang__)
#   if defined(__APPLE__)
#       define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with apple-clang " _XO_MATH_STRINGIFY(__clang_major__) "." _XO_MATH_STRINGIFY(__clang_minor__) "." _XO_MATH_STRINGIFY(__clang_patchlevel__) ", supporting simd: " XO_MATH_HIGHEST_SIMD "."
#   else
#       define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with clang " _XO_MATH_STRINGIFY(__clang_major__) "." _XO_MATH_STRINGIFY(__clang_minor__) "." _XO_MATH_STRINGIFY(__clang_patchlevel__) ", supporting simd: " XO_MATH_HIGHEST_SIMD "."
#   endif
// pragma message is a warning on clang.
//#   pragma message XO_MATH_COMPILER_INFO
#elif defined(__GNUC__)
#   define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with gcc " _XO_MATH_STRINGIFY(__GNUC__) "." _XO_MATH_STRINGIFY(__GNUC_MINOR__) "." _XO_MATH_STRINGIFY(__GNUC_PATCHLEVEL__) ", supporting simd: " XO_MATH_HIGHEST_SIMD "."
#else
#   define XO_MATH_COMPILER_INFO "xo-math v" XO_MATH_VERSION_STR " is compiled with an unknown compiler, supporting simd: " XO_MATH_HIGHEST_SIMD "."
#endif

#endif // XO_MATH_H

