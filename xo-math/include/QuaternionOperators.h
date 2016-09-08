#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include QuaternionOperators.h directly. Include GameMath.h, which fully implements this type.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

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
    return (_mm_movemask_ps(_mm_cmplt_ps(sse::Abs(_mm_sub_ps(q.m, m)), sse::Epsilon)) & 0b1111) == 0b1111;
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

XOMATH_END_XO_NS

#endif