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

//! A two dimensional euclidean vector, optimized for use in games.
//! @sa https://en.wikipedia.org/wiki/Euclidean_vector
class _XOSIMDALIGN Vector2 {
public:

    //>See
    //! @name Constructors
    //! @{
    Vector2() { } //!< Performs no initialization.
    Vector2(float v); //!< All elements are set to f.
    Vector2(float x, float y); //!< Assigns each named value accordingly.
    Vector2(const Vector2& v); //!< Copy constructor, trivial.
    Vector2(const class Vector3& v); //!< Assigns same-name values from v.
    Vector2(const class Vector4& v); //!< Assigns same-name values from v.
    //! @}


    //>See
    //! @name Set / Get Methods
    //! @{

    //! Set all. x, and y will be assigned to the input params.
    Vector2& Set(float x, float y);
    //! Set each. x, and y will be assigned to f.
    Vector2& Set(float f);
    //! Set each. Copies vec into this.
    Vector2& Set(const Vector2& vec);
    //! Extract all getter. x and y will be assigned to those values of this vector.
    void Get(float& x, float& y) const;
    //! Extract all getter. f[0] and f[1] will be assigned to x and y respectively.
    void Get(float* f) const;
    //! @}

    //>See
    //! @name Special Operators
    //! @{

    //! Extract reference operator, useful for setting values by index.
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\? & undefined\end{cases}\f]
    _XOINL float& operator [](int i);
    //! Extract const reference operator, useful for getting values by index.
    //! \f[i\begin{cases}0 & return\ x;\\1 & return\ y;\\? & undefined\end{cases}\f]
    _XOINL const float& operator [](int i) const;
    //! Negate operator. Returns a vector with all elements with a flipped sign:
    //!
    //! \f$\begin{pmatrix}-x,&-y\end{pmatrix}\f$
    _XOINL Vector2 operator - () const;
    //! Returns this vector swizzled so elements appear in reverse order.
    //!
    //! \f$\begin{pmatrix}&y,&x\end{pmatrix}\f$
    _XOINL Vector2 operator ~ () const;
    //! @}


    //>See
    //! @name Math Operators
    //! Operates on all same-name vector elements with other vector types, or all elements to scalar/integer types.
    //! @{

    _XOINL Vector2& operator += (const Vector2& v);
    _XOINL Vector2& operator += (float v);
    _XOINL Vector2& operator += (double v);
    _XOINL Vector2& operator += (int v);
    _XOINL Vector2& operator += (const class Vector3& v);
    _XOINL Vector2& operator += (const class Vector4& v);
    _XOINL Vector2& operator -= (const Vector2& v);
    _XOINL Vector2& operator -= (float v);
    _XOINL Vector2& operator -= (double v);
    _XOINL Vector2& operator -= (int v);
    _XOINL Vector2& operator -= (const class Vector3& v);
    _XOINL Vector2& operator -= (const class Vector4& v);
    _XOINL Vector2& operator *= (const Vector2& v);
    _XOINL Vector2& operator *= (float v);
    _XOINL Vector2& operator *= (double v);
    _XOINL Vector2& operator *= (int v);
    _XOINL Vector2& operator *= (const class Vector3& v);
    _XOINL Vector2& operator *= (const class Vector4& v);
    _XOINL Vector2& operator /= (const Vector2& v);
    _XOINL Vector2& operator /= (float v);
    _XOINL Vector2& operator /= (double v);
    _XOINL Vector2& operator /= (int v);
    _XOINL Vector2& operator /= (const class Vector3& v);
    _XOINL Vector2& operator /= (const class Vector4& v);
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
    //! @}

    //>See
    //! @name Comparison Operators
    //! When comparing (<, >, <=, >=) against other vectors, the square magnitude is compared.
    //! 
    //! When comparing (<, >, <=, >=) against scalars, the scalar is squared and compared to the SquareMagnitude() of this vector.
    //!
    //! When comparing equality (==, !=) against other vectors, each same name element is compared. If each element has a 
    //! difference of <= Vector2::Epsilon the vector is considered equal.
    //!
    //! When comparing equality (==, !=) against scalars, the scalar is squared and compared to the SquareMagnitude() of this vector.
    //! @{
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
    //! @}

    //>See
    //! @name Methods
    //! @{

    //! Returns true when the Magnitude of this vector is within Vector2::Epsilon of being 1.0
    bool IsNormalized() const {
        return CloseEnough(MagnitudeSquared(), 1.0f, Epsilon);
    }
    //! Returns true when the Magnitude of this vector is <= Vector2::Epsilon
    bool IsZero() const {
        return CloseEnough(MagnitudeSquared(), 0.0f, Epsilon);
    }
    //! The length of this vector
    //! It's preferred to use Vector2::MagnitudeSquared when possible, as Vector2::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert = \sqrt{(x\times x)+(y\times y)}\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    float Magnitude() const {
        return sqrtf(MagnitudeSquared());
    }
    //! The square length of this vector.
    //! It's preferred to use Vector2::MagnitudeSquared when possible, as Vector2::Magnitude requires a call to Sqrt.
    //!
    //! \f$\lvert\rvert\boldsymbol{this}\lvert\rvert^2 = (x\times x)+(y\times y)\f$
    //! @sa https://en.wikipedia.org/wiki/Magnitude_(mathematics)#Euclidean_vector_space
    float MagnitudeSquared() const {
        return x*x + y*y;
    }
    //! The sum of all vector elements.
    //!
    //! \f$x+y\f$
    float Sum() const {
        return x + y;
    }

    //! Normalizes this vector to a Magnitude of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    Vector2& Normalize() {
        return (*this) /= Magnitude();
    }

    //! Returns a copy of this vector with a Magnitude of 1.
    //! @sa https://en.wikipedia.org/wiki/Unit_vector
    Vector2 Normalized() const {
        return Vector2(*this).Normalize();
    }
    //! @}

    //>See
    //! @name Static Methods
    //! @{

    //! Sets outVec to a vector interpolated between a and b by a scalar amount t.
    //! @sa https://en.wikipedia.org/wiki/Linear_interpolation
    static void Lerp(const Vector2& a, const Vector2& b, float t, Vector2& outVec) {
        outVec = a + ((b - a) * t);
    }
    //! Set outVec to have elements equal to the max of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\max(a.x, b.x)&\max(a.y, b.y)\end{pmatrix}\f$
    static void Max(const Vector2& a, const Vector2& b, Vector2& outVec) {
        outVec.Set(_XO_MAX(a.x, b.x), _XO_MAX(a.y, b.y));
    }
    //! Assigns outvec to a vector half way between a and b.
    static void Midpoint(const Vector2& a, const Vector2& b, Vector2& outVec) {
        Vector2::Lerp(a, b, 0.5f, outVec);
    }
    //! Set outVec to have elements equal to the min of each element in a and b.
    //!
    //! \f$\begin{pmatrix}\min(a.x, b.x)&\min(a.y, b.y)\end{pmatrix}\f$
    static void Min(const Vector2& a, const Vector2& b, Vector2& outVec) {
        outVec.Set(_XO_MIN(a.x, b.x), _XO_MIN(a.y, b.y));
    }
    //! Assigns outVec to a vector rotated 90 degrees clockwise from v.
    static void OrthogonalCCW(const Vector2& v, Vector2& outVec) {
        outVec.Set(-v.y, v.x);
    }
    //! Assigns outVec to a vector rotated 90 degrees counterclockwise from v.
    static void OrthogonalCW(const Vector2& v, Vector2& outVec) {
        outVec.Set(v.y, -v.x);
    }

    //! Calls Vector2::AngleRadians, converting the return value to degrees.
    static float AngleDegrees(const Vector2& a, const Vector2& b) {
        return AngleRadians(a, b) * Rad2Deg;
    }
    //! Returns the angle in radians between vector a and b. Note that this value is counterclockwise.
    static float AngleRadians(const Vector2& a, const Vector2& b) {
        return -ATan2(Cross(a, b), Dot(a, b));
    }
    //! A 2D cross product returns a magnitude of what would be the z axis
    //! in the 3d cross product of a and b, where z. Note that only the magnitude
    //! is significant in the return value as x and y values of this product would be zero.
    //! @sa Vector3::Cross
    static float Cross(const Vector2& a, const Vector2& b) {
        return (a.x * b.y) - (a.y * b.x);
    }
    //! Returns the distance between vectors a and b in 2 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance = \lvert\rvert\boldsymbol{b-a}\lvert\rvert\f$
    static float Distance(const Vector2& a, const Vector2& b) {
        return (b - a).Magnitude();
    }
    //! Returns the square distance between vectors a and b in 2 dimensional space.
    //! It's preferred to use the DistanceSquared when possible, as Distance requires a call to Sqrt.
    //!
    //! \f$distance^2 = \lvert\rvert\boldsymbol{b-a}\lvert\rvert^2\f$
    static float DistanceSquared(const Vector2& a, const Vector2& b) {
        return (b - a).MagnitudeSquared();
    }
    //! Returns a single number representing a product of magnitudes. Commonly used with two normalized 
    //! vectors to determine if they are pointed the same way. In this case: 1.0 represents same-facing vectors
    //! 0 represents perpendicular vectors, and -1 will be facing away
    //!
    //! \f$a\cdot b =(a.x\times b.x) + (a.y\times b.y)\f$
    //!
    //! @sa https://en.wikipedia.org/wiki/Dot_product
    static float Dot(const Vector2& a, const Vector2& b) {
        return (a * b).Sum();
    }
    //! @}

#define _RET_VARIANT(name) { Vector2 tempV; name(
#define _RET_VARIANT_END() tempV); return tempV; }
#define _RET_VARIANT_0(name)                                 _RET_VARIANT(name)                               _RET_VARIANT_END()
#define _RET_VARIANT_1(name, first)                          _RET_VARIANT(name) first,                        _RET_VARIANT_END()
#define _RET_VARIANT_2(name, first, second)                  _RET_VARIANT(name) first, second,                _RET_VARIANT_END()
#define _RET_VARIANT_3(name, first, second, third)           _RET_VARIANT(name) first, second, third,         _RET_VARIANT_END()
#define _RET_VARIANT_4(name, first, second, third, fourth)   _RET_VARIANT(name) first, second, third, fourth, _RET_VARIANT_END()
#define _THIS_VARIANT0(name)                                { return name(*this); }
#define _THIS_VARIANT1(name, first)                         { return name(*this, first); }
#define _THIS_VARIANT2(name, first, second)                 { return name(*this, first, second); }

    //>See
    //! @name Variants
    //! Variants of other same-name static methods. See their documentation for more details under the 
    //! Static Methods heading.
    //!
    //! Non static variants replace the first Vector2 parameter by 'this' vector.
    //! Static variants return what would have been the outVec param.
    //! @{

    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t)    _RET_VARIANT_3(Lerp, a, b, t)
    static Vector2 Max(const Vector2& a, const Vector2& b)              _RET_VARIANT_2(Max, a, b)
    static Vector2 Midpoint(const Vector2& a, const Vector2& b)         _RET_VARIANT_2(Midpoint, a, b)
    static Vector2 Min(const Vector2& a, const Vector2& b)              _RET_VARIANT_2(Min, a, b)
    static Vector2 OrthogonalCCW(const Vector2& v)                      _RET_VARIANT_1(OrthogonalCCW, v)
    static Vector2 OrthogonalCW(const Vector2& v)                       _RET_VARIANT_1(OrthogonalCW, v)

    float AngleDegrees(const Vector2& v) const                          _THIS_VARIANT1(AngleDegrees, v)
    float AngleRadians(const Vector2& v) const                          _THIS_VARIANT1(AngleRadians, v)
    float Cross(const Vector2& v) const                                 _THIS_VARIANT1(Cross, v)
    float Distance(const Vector2& v) const                              _THIS_VARIANT1(Distance, v)
    float DistanceSquared(const Vector2& v) const                       _THIS_VARIANT1(DistanceSquared, v)
    float Dot(const Vector2& v) const                                   _THIS_VARIANT1(Dot, v)
    Vector2 Lerp(const Vector2& v, float t) const                       _THIS_VARIANT2(Lerp, v, t)
    Vector2 Midpoint(const Vector2& v) const                            _THIS_VARIANT1(Midpoint, v)
    Vector2 OrthogonalCCW() const                                       _THIS_VARIANT0(OrthogonalCCW)
    Vector2 OrthogonalCW() const                                        _THIS_VARIANT0(OrthogonalCW)
    //! @}

#undef _RET_VARIANT
#undef _RET_VARIANT_END
#undef _RET_VARIANT_0
#undef _RET_VARIANT_1
#undef _RET_VARIANT_2
#undef _RET_VARIANT_3
#undef _RET_VARIANT_4
#undef _THIS_VARIANT0
#undef _THIS_VARIANT1
#undef _THIS_VARIANT2

    //>See
    //! @name Extras
    //! @{

    //! @todo Make this optional with a define.
    //! Prints the contents of vector v and its magnitude to the provided ostream.
#ifndef XO_NO_OSTREAM
    friend std::ostream& operator <<(std::ostream& os, const Vector2& v) {
        os << "(x:" << v.x << ", y:" << v.y << ", mag:" << v.Magnitude() << ")";
        return os;
    }
#endif
    //! @}

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