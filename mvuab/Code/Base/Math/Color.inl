#include "MathUtils.h"

inline CColor& Math::CColor::Clamp()
{
    r = Math::Utils::Min(Math::Utils::Max(r, 0.0f), 1.0f);
    g = Math::Utils::Min(Math::Utils::Max(g, 0.0f), 1.0f);
    b = Math::Utils::Min(Math::Utils::Max(b, 0.0f), 1.0f);
    a = Math::Utils::Min(Math::Utils::Max(a, 0.0f), 1.0f);

    return (*this);
}

///
/// Construye un uint32 que codifica el color como 0xAARRGGBB (PC)
///
inline uint32 Math::CColor::GetUint32Argb () const
{
    return  (uint32)(float32ToByte(a) << 24)
            + (float32ToByte(r) << 16)
            + (float32ToByte(g) << 8)
            +  float32ToByte(b);
}

///
/// Construye un uint32 que codifica el color como 0xAABBGGRR (PC)
///
inline uint32 Math::CColor::GetUint32Abgr () const
{
    return  (uint32)(float32ToByte(a) << 24)
            + (float32ToByte(b) << 16)
            + (float32ToByte(g) << 8)
            +  float32ToByte(r);
}

///
/// Asigna el color a partir de un uint32 en formato  0xAARRGGBB
///
inline void  Math::CColor::SetUint32Argb (int32 argb)
{
    a = ByteTofloat32(static_cast<uint8>(     (argb >> 24))); // alpha
    r = ByteTofloat32(static_cast<uint8>(0xff & (argb >> 16))); // red
    g = ByteTofloat32(static_cast<uint8>(0xff & (argb >> 8))); // green
    b = ByteTofloat32(static_cast<uint8>(0xff & (argb    ))); // blue
}

///
/// Asigna el color a partir de un uint32 en formato  0xAABBGGRR
///
inline void  Math::CColor::SetUint32Abgr (int32 abgr)
{
    a = ByteTofloat32(static_cast<uint8>(     (abgr >> 24))); // alpha
    b = ByteTofloat32(static_cast<uint8>(0xff & (abgr >> 16))); // blue
    g = ByteTofloat32(static_cast<uint8>(0xff & (abgr >> 8))); // green
    r = ByteTofloat32(static_cast<uint8>(0xff & (abgr    ))); // red
}


// Funci??n de utilidad para convertir una componente de color en coma flotante a su versi??n de un byte (0..255)
inline uint8 CColor::float32ToByte(float32 fValue) const
{
    int i = static_cast<int>(0xff * fValue);
    i = Math::Utils::Clamp(i, 0, 0xff);

    return static_cast<uint8>(i);
}

// Funci??n de utilidad para convertir un byte con una componente de color (0..255) a componente float32 (0.0..1.0)
inline float32 Math::CColor::ByteTofloat32 (uint8 uValue) const
{
    return static_cast<float32>(uValue) / 255.0f;
}

inline Math::CColor& Math::CColor::Lerp(const Math::CColor& other, float32 f)
{
    r += (other.r - r) * f;
    g += (other.g - g) * f;
    b += (other.b - b) * f;
    a += (other.a - a) * f;
    return *this;
}

// Operadores aritméticos
inline Math::CColor Math::CColor::operator + (const Math::CColor& otro)    const
{
    return Math::CColor(otro.r + r, otro.g + g, otro.b + b, otro.a + a);
}

inline Math::CColor Math::CColor::operator - (const Math::CColor& otro)    const
{
    return Math::CColor(otro.r - r, otro.g - g, otro.b - b, otro.a - a);
}

inline Math::CColor Math::CColor::operator * (const Math::CColor& otro)    const
{
    return Math::CColor(otro.r * r,
                        otro.g * g,
                        otro.b * b,
                        otro.a * a);
}

inline Math::CColor Math::CColor::operator * (float32 escalar) const
{
    return Math::CColor( r * escalar, g * escalar, b * escalar, a * escalar );
}

inline Math::CColor& Math::CColor::operator += (const Math::CColor& otro)
{
    r += otro.r;
    g += otro.g;
    b += otro.b;
    a += otro.a;
    return (*this);
}

inline Math::CColor& Math::CColor::operator -= (const Math::CColor & otro)
{
    r -= otro.r;
    g -= otro.g;
    b -= otro.b;
    a -= otro.a;
    return (*this);
}

inline Math::CColor& Math::CColor::operator *= (const Math::CColor & otro)
{
    r *= otro.r;
    g *= otro.g;
    b *= otro.b;
    a *= otro.a;
    return (*this);
}

inline Math::CColor& Math::CColor::operator *= (float32 escalar)
{
    r *= escalar;
    g *= escalar;
    b *= escalar;
    a *= escalar;
    return (*this);
}
