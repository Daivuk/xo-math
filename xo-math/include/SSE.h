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

#ifndef XOMATH_INTERNAL
static_assert(false, "Don't include SSE.h directly. Include GameMath.h.");
#else // XOMATH_INTERNAL

XOMATH_BEGIN_XO_NS

_XOINL
Vector2 Abs(const Vector2& v) {
    return Vector2(Abs(v.x), Abs(v.y));
}

#if XO_SSE2 // TODO: check if SSE2 is required
_XOINL
Vector3 Abs(const Vector3& v) {
    return (sse::Abs(v.m));
}

_XOINL
Vector4 Abs(const Vector4& v) {
    return Vector4(sse::Abs(v.m));
}
#endif

#if XO_SSE

namespace sse {

    // The control of MXCSR usage is inspired by Agner Fog's use of them in vectormath.
    // vectormath uses them to optionally speed up subnormal operations.
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

        // TODO: ifdef for whichever sse version this came in on.
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

    volatile _XOTLS unsigned LastKnownControlWord = 0;

    void UpdateControlWord() {
        LastKnownControlWord = _mm_getcsr();
    }

    void SetControlWord(unsigned control) {
        _mm_setcsr(LastKnownControlWord = control);
    }

    void SetControlWordAddative(unsigned control) {
        _mm_setcsr(LastKnownControlWord |= control);
    }

    void RemoveControlWord(unsigned control) {
        _mm_setcsr(LastKnownControlWord &= ~control);
    }

    bool HasControlFlagBeenSet(unsigned flags, bool withUpdate = false, bool thenFlush = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if((LastKnownControlWord & flags) == flags) {
            if(thenFlush) {
                RemoveControlWord(flags);
            }
            return true;
        }
        return false;
    }

    bool HasControlFlagBeenSet(mxcsr::Flags flags, bool withUpdate = false, bool thenFlush = false) {
        return HasControlFlagBeenSet((unsigned)flags, withUpdate, thenFlush);
    }

    bool HasInvalidOperationExceptionOccured(bool withUpdate = false, bool thenFlush = false) {
        return HasControlFlagBeenSet(mxcsr::Flags::InvalidOperation, withUpdate, thenFlush);
    }

    bool HasDenormalExceptionOccured(bool withUpdate = false, bool thenFlush = false) {
        return HasControlFlagBeenSet(mxcsr::Flags::Denormal, withUpdate, thenFlush);
    }

    bool HasDivideByZeroExceptionOccured(bool withUpdate = false, bool thenFlush = false) {
        return HasControlFlagBeenSet(mxcsr::Flags::DivideByZero, withUpdate, thenFlush);
    }

    bool HasOverflowExceptionOccured(bool withUpdate = false, bool thenFlush = false) {
        return HasControlFlagBeenSet(mxcsr::Flags::Overflow, withUpdate, thenFlush);
    }

    bool HasUnderflowExceptionOccured(bool withUpdate = false, bool thenFlush = false) {
        return HasControlFlagBeenSet(mxcsr::Flags::Underflow, withUpdate, thenFlush);
    }

    bool HasPrecisionExceptionOccured(bool withUpdate = false, bool thenFlush = false) {
        return HasControlFlagBeenSet(mxcsr::Flags::Precision, withUpdate, thenFlush);
    }

    void SetControlMask(unsigned mask, bool value, bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if(value) {
            SetControlWordAddative(mask);
        }
        else {
            RemoveControlWord(mask);
        }
    }

    void SetControlMask(mxcsr::Masks mask, bool value, bool withUpdate = false) {
        SetControlMask((unsigned)mask, value, withUpdate);
    }

    bool GetControlMask(unsigned mask, bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (LastKnownControlWord & mask) == mask;
    }

    bool GetControlMask(mxcsr::Masks mask, bool withUpdate = false) {
        return GetControlMask((unsigned)mask, withUpdate);
    }

    void SetInvalidOperationExceptionMask(bool value, bool withUpdate = false) {
        SetControlMask(mxcsr::Masks::InvalidOperation, value, withUpdate);
    }

    void SetDenormalExceptionMask(bool value, bool withUpdate = false) {
        SetControlMask(mxcsr::Masks::Denormal, value, withUpdate);
    }

    void SetDivideByZeroExceptionMask(bool value, bool withUpdate = false) {
        SetControlMask(mxcsr::Masks::DivideByZero, value, withUpdate);
    }

    void SetOverflowExceptionMask(bool value, bool withUpdate = false) {
        SetControlMask(mxcsr::Masks::Overflow, value, withUpdate);
    }

    void SetUnderflowExceptionMask(bool value, bool withUpdate = false) {
        SetControlMask(mxcsr::Masks::Underflow, value, withUpdate);
    }

    void SetPrecisionExceptionMask(bool value, bool withUpdate = false) {
        SetControlMask(mxcsr::Masks::Precision, value, withUpdate);
    }

    void ThrowAllExceptions(bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        SetInvalidOperationExceptionMask(false);
        SetDenormalExceptionMask(false);
        SetDivideByZeroExceptionMask(false);
        SetOverflowExceptionMask(false);
        SetUnderflowExceptionMask(false);
        SetPrecisionExceptionMask(false);
    }

    void ThrowNoExceptions(bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        SetInvalidOperationExceptionMask(true);
        SetDenormalExceptionMask(true);
        SetDivideByZeroExceptionMask(true);
        SetOverflowExceptionMask(true);
        SetUnderflowExceptionMask(true);
        SetPrecisionExceptionMask(true);
    }

    bool GetInvalidOperationExceptionMask(bool withUpdate = false) {
        return GetControlMask(mxcsr::Masks::InvalidOperation, withUpdate);
    }

    bool GetDenormalExceptionMask(bool withUpdate = false) {
        return GetControlMask(mxcsr::Masks::Denormal, withUpdate);
    }

    bool GetDivideByZeroExceptionMask(bool withUpdate = false) {
        return GetControlMask(mxcsr::Masks::DivideByZero, withUpdate);
    }

    bool GetOverflowExceptionMask(bool withUpdate = false) {
        return GetControlMask(mxcsr::Masks::Overflow, withUpdate);
    }

    bool GetUnderflowExceptionMask(bool withUpdate = false) {
        return GetControlMask(mxcsr::Masks::Underflow, withUpdate);
    }

    bool GetPrecisionExceptionMask(bool withUpdate = false) {
        return GetControlMask(mxcsr::Masks::Precision, withUpdate);
    }

    mxcsr::Rounding GetRoundingMode(bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (mxcsr::Rounding)(LastKnownControlWord & (unsigned)mxcsr::Rounding::Bits);
    }

    void SetRoundingMode(unsigned mode, bool withUpdate = false) {
        mode &= (unsigned)mxcsr::Rounding::Bits;
        if(withUpdate) {
            UpdateControlWord();
        }
        SetControlWordAddative(mode);
    }

    void SetRoundingMode(mxcsr::Rounding mode, bool withUpdate = false) {
        SetRoundingMode((unsigned)mode, withUpdate);
    }

    bool HasDenormalsAreZeroSet(bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (LastKnownControlWord & (unsigned)mxcsr::DAZ::DenormalsAreZero) == (unsigned)mxcsr::DAZ::DenormalsAreZero;
    }

    void SetDenormalsAreZero(bool value, bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if(value) {
            SetControlWordAddative((unsigned)mxcsr::DAZ::DenormalsAreZero);
        }
        else {
            RemoveControlWord((unsigned)mxcsr::DAZ::DenormalsAreZero);
        }
    }

    bool HasFlushToZeroSet(bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        return (LastKnownControlWord & (unsigned)mxcsr::FZ::FlushToZero) == (unsigned)mxcsr::FZ::FlushToZero;
    }

    void SetFlushToZero(bool value, bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        if(value) {
            SetControlWordAddative((unsigned)mxcsr::FZ::FlushToZero);
        }
        else {
            RemoveControlWord((unsigned)mxcsr::FZ::FlushToZero);
        }
    }

    void GetAllMXCSRInfo(std::ostream& os, bool withUpdate = false) {
        if(withUpdate) {
            UpdateControlWord();
        }
        os << "MXCSR rounding:\n";
        os << "\t";
        switch(GetRoundingMode()) {
            case mxcsr::Rounding::Nearest:
                os << "Nearest";
                break;
            case mxcsr::Rounding::Positive:
                os << "Positive";
                break;
            case mxcsr::Rounding::Negative:
                os << "Negative";
                break;
            case mxcsr::Rounding::Zero:
                os << "Zero";
                break;
        }
        os << "\n";
        os << "MXCSR masks:\n";
        os << "\t" << "InvalidOperationException: " << GetInvalidOperationExceptionMask() << "\n"; 
        os << "\t" << "DenormalException: " << GetDenormalExceptionMask() << "\n"; 
        os << "\t" << "DivideByZeroException: " << GetDivideByZeroExceptionMask() << "\n"; 
        os << "\t" << "OverflowException: " << GetOverflowExceptionMask() << "\n"; 
        os << "\t" << "UnderflowException: " << GetUnderflowExceptionMask() << "\n"; 
        os << "\t" << "PrecisionException: " << GetPrecisionExceptionMask() << "\n"; 
        os << "\t" << "DenormalsAreZero: " << HasDenormalsAreZeroSet() << "\n";
        os << "\t" << "FlushToZero: " << HasFlushToZeroSet() << "\n";
        os << "MXCSR flags:\n";
        os << "\t" << "InvalidOperationException: " << HasInvalidOperationExceptionOccured() << "\n"; 
        os << "\t" << "DenormalException: " << HasDenormalExceptionOccured() << "\n"; 
        os << "\t" << "DivideByZeroException: " << HasDivideByZeroExceptionOccured() << "\n"; 
        os << "\t" << "OverflowException: " << HasOverflowExceptionOccured() << "\n"; 
        os << "\t" << "UnderflowException: " << HasUnderflowExceptionOccured() << "\n"; 
        os << "\t" << "PrecisionException: " << HasPrecisionExceptionOccured() << "\n";
    }
}
#endif

XOMATH_END_XO_NS

#endif // XOMATH_INTERNAL